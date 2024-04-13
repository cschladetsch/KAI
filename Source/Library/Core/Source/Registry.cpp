#include <utility>
#include <cassert>

#include <KAI/Core/Registry.h>
#include <KAI/Core/BuiltinTypes/Signed32.h>
#include <KAI/Core/Memory/StandardAllocator.h>
#include <KAI/Core/Object/Class.h>
#include <KAI/Core/Object/IObject.h>

#include <KAI/Core/Tree.h>
#include <KAI/Core/TriColor.h>

// Use tri-color generational gc. See https://en.wikipedia.org/wiki/Tracing_garbage_collection#Tri-color_marking
#define KAI_USE_TRICOLOR

KAI_BEGIN

Registry::Registry()
{
    _allocator = std::make_shared<Memory::StandardAllocator>();
    Construct();
}

Registry::Registry(std::shared_ptr<Memory::IAllocator> alloc)
{
    _allocator = std::move(alloc);
    Construct();
}

void Registry::Construct()
{
    _classes.resize(2000, nullptr);
    gc_trace_level = 1;
    _tree = nullptr;
    std::fill(_classes.begin(), _classes.end(), static_cast<ClassBase const*>(nullptr));
}

Registry::~Registry()
{
    Clear();
}

void Registry::Clear()
{
    ClearInstances();
}

void Registry::ClearInstances()
{
    // Create a set of handles to destroy, then destroy them
    // this can't be done in one pass, as otherwise we would be mutating the
    // container as we traverse it.
    std::vector<Handle> handles;
    for (auto const &instance : _instances)
        handles.push_back(instance.first);

    for (auto const &handle : handles)
        DestroyObject(handle, true);

    _instances.clear();
}

void Registry::NominateAll()
{
    _deathRow.clear();
    for (const auto & [handle, _] : _instances)
        _deathRow.insert(handle);
}

Object Registry::NewFromTypeNumber(Type::Number type_number)
{
    ClassBase const *klass = _classes[type_number.ToInt()];
    if (!klass)
        KAI_THROW_1(UnknownTypeNumber, type_number.ToInt());

    return NewFromClass(klass);
}

Object Registry::NewFromClassName(const char *classname_str)
{
    const Label className(classname_str);
    const ClassBase *klass = GetClass(className);
    if (klass == nullptr)
        KAI_THROW_1(UnknownClass<>, String(classname_str));

    return NewFromClass(klass);
}

const ClassBase *Registry::GetClass(const Label &name)
{
    for (auto const klass : _classes)
        if (klass && klass->GetName() == name)
            return klass;

    return nullptr;
}

void Registry::DestroyNominated()
{
    // Copy the elements in death row because when we delete
    // objects, they may release other objects.
    DeathRow copy = _deathRow;
    _deathRow.clear();
    for (auto const &ob : copy)
        DestroyObject(ob);
}

void Registry::DestroyObject(Handle handle, bool force)
{
    bool succeeded = false;
    KAI_TRY
    {
        const auto found = _instances.find(handle);
        if (found == _instances.end())
        {
#ifdef KAI_DEBUG_REGISTRY
            if (IsWatching(handle))
            {
                KAI_TRACE() << handle << ": doesn't exist, not deleted";
            }
#endif
            return;
        }

        StorageBase &base = *found->second;
        assert(base.GetHandle() == handle);
        if (!base.IsManaged() && !force)
        {
#ifdef KAI_DEBUG_REGISTRY
            if (IsWatching(handle))
                KAI_TRACE() << handle << ": " << base << " is not managed, not deleted";
#endif
            return;
        }

#ifdef KAI_DEBUG_REGISTRY
        bool trace = gc_trace_level > 3;
        trace = trace || IsWatching(base);
        if (trace)
        {
            KAI_TRY
            {
                KAI_TRACE() << handle;
            }
            KAI_CATCH_ALL()
            {
            }
        }
#endif

#ifdef KAI_USE_TRICOLOR
        // When using tri-color GC, objects are always deleted when they are destroyed.
        if (!base.GetClass()->Destroy(base) && !force)
        {
            base.SetColor(ObjectColor::Grey);
            return;
        }
#endif
        base.GetClass()->Delete(base);
        _instances.erase(found);

        if (const auto retained = _retainedObjects.find(handle); retained != _retainedObjects.end())
            _retainedObjects.erase(retained);

        succeeded = true;
    }
    KAI_CATCH(Exception::Base, E)
    {
        KAI_UNUSED(E);
        KAI_TRACE() << "\n\t" << E.ToString();
    }
    KAI_CATCH(std::exception, E)
    {
        KAI_UNUSED(E);
        KAI_TRACE() << "std::exception: " << E.what();
    }
    KAI_CATCH_ALL()
    {
    }

    if (!succeeded)
    {
        KAI_TRACE_WARN() << " coudldn't delete handle " << handle;
        if (auto const found = _instances.find(handle); found != _instances.end())
        {
            // this leaks and has other *TERRIBLE* consequences but it is the best we can do to keep afloat
            KAI_TRACE_ERROR() << "failed to delete " << handle;
            _instances.erase(found);
        }
    }
}

void Registry::PruneRetained()
{
    auto retained = _retainedObjects.begin();
    const auto end = _retainedObjects.end();
    while (retained != end)
    {
        if (Object object = GetObject(*retained); !object.Exists())
            retained = _retainedObjects.erase(retained);
        else
            ++retained;
    }
}

const ClassBase *Registry::GetClass(Type::Number type_number)
{
    const auto tn = type_number.ToInt();
    if (tn >= static_cast<int>(_classes.size()))
        KAI_THROW_1(LogicError, "Inalid type number");

    return _classes[tn];
}

StorageBase *Registry::GetStorageBase(Handle handle) const
{
    if (handle == Handle(0))
        return nullptr;

    const auto obj = _instances.find(handle);
    if (obj == _instances.end())
        return nullptr;

    return obj->second;
}

bool Registry::OnDeathRow(Handle handle) const
{
    return _deathRow.find(handle) != _deathRow.end();
}

void Registry::AddClass(const ClassBase *klass)
{
    if (klass == nullptr)
        KAI_THROW_0(NullObject);

    if (GetClass(klass->GetTypeNumber()))
        KAI_THROW_1(Base, "Duplicate Class");

    _classes[klass->GetTypeNumber().ToInt()] = klass;
}

Object Registry::GetObject(Handle handle) const
{
#ifdef KAI_DEBUG_REGISTRY
    if (IsWatching(handle) && gc_trace_level > 1)
        KAI_TRACE() << handle;
#endif

    if (handle == Handle(0))
        return Object();

    const auto found = _instances.find(handle);
    if (found == _instances.end())
        return Object();

    return static_cast<Object>(*found->second);
}

void Registry::MarkSweepAndDestroy(Object root)
{
#ifdef KAI_USE_TRICOLOR
    AddRoot(root);
    TriColor();
    return;
#else
    DestroyNominated();            // destroy any pending
    MarkAndSweep(_root);            // mark objects for destruction
    DestroyNominated();            // destroy them
#endif
}

void Registry::MarkAndSweep(Object root)
{
    Mark(root);
    Sweep();
}

void Registry::Mark(Object root)
{
    // Mark everything that is reachable from the given object _root.
    if (root.Exists())
        MarkAll(root.GetStorageBase(), true);
}

void Registry::MarkAll(StorageBase &root, bool marked) const
{
    MarkObject(root, marked);

    const Dictionary &dict = root.GetDictionary();
    for (const auto &[_, child] : dict)
    {
        StorageBase *base = GetStorageBase(child.GetHandle());
        if (base == nullptr)
            continue;

        if (base->IsManaged())
            MarkObject(*base, marked);

        if (base->GetSwitches() & IObject::NoRecurse)
            continue;

        MarkAll(*base, marked);
    }
}

void Registry::Sweep()
{
    if (_instances.empty())
        return;

    auto A = _instances.begin(), B = _instances.end();
    // the handle of the next object created
    const auto last = Handle(_nextHandle.GetValue());
    for (; A != B; ++A)
    {
        StorageBase *base = A->second;

        // do not consider objects that were created during this loop!
        if (last < base->GetHandle())
            continue;

        if (!base->IsManaged())
            continue;

        if (!base->IsMarked())
            Delete(A->first);
        else 
            base->SetMarked(false);
    }
}

void Registry::Delete(Handle handle)
{
#ifdef KAI_USE_TRICOLOR
    KAI_NOT_IMPLEMENTED_1("Delete when using garbage collection.");
#else

#ifdef KAI_DEBUG_REGISTRY
    if (IsObserving(handle))
    {
        KAI_TRACE() << "Deleting " << handle;
    }
#endif
    // if unknown handle, do nothing
    Instances::const_iterator instance = _instances.find(handle);
    if (instance == _instances.end())
        return;        

    // detach from parent
    if (instance->second)
    {
        StorageBase &storage = *instance->second;
        storage.SetColor(ObjectColor::White);
        storage.SetColorRecursive(ObjectColor::White);
        storage.GetClass()->Destroy(storage);
        //Detach(storage);
    }

    // remove from list of _retained objects
    auto etained = _retainedObjects.find(handle);
    if (_retained != _retainedObjects.end())
        _retainedObjects.erase(_retained);

    // mark for pending collection
#ifndef KAI_USE_TRICOLOR
    _deathRow.insert(handle);
#endif
#endif
}

void Registry::Delete(Object const &object)
{
#ifdef KAI_DEBUG_REGISTRY
    if (IsWatching(object))
        KAI_TRACE() << object.GetHandle();
#endif
    Delete(object.GetHandle());
}

Pointer<ClassBase const *> Registry::AddClass(Type::Number N, ClassBase const *K)
{
    _classes[N.ToInt()] = K;
    return Pointer<ClassBase const *>();
}

Registry::Percentage Registry::CalcMemoryUsage() const { return 0; }
Registry::Percentage Registry::CalcMemoryFragmentationPercentage() const { return 0; }
void Registry::DefragmentMemory() {}
bool Registry::Pin(Handle) { return false; }
bool Registry::Unpin(Handle) { return false; }

void Registry::GarbageCollect()
{
    if (!_tree)
    {
        KAI_TRACE_WARN() << "GC requested with no tree";
        return;
    }

    GarbageCollect(_tree->GetRoot());
}

Object Registry::NewFromClass(const ClassBase *klass)
{
    if (klass == nullptr)
        KAI_THROW_1(UnknownClass<>, "NULL Class");

    const Handle handle(_nextHandle.NextValue());
    StorageBase *base = nullptr;
    base = klass->NewStorage(this, handle);

#ifdef KAI_DEBUG_REGISTRY
    if (IsWatchingType(klass->GetTypeNumber()))
        KAI_TRACE() << klass->GetName() << ": " << handle;
#endif

    base->SetColor(ObjectColor::White);
    base->SetMarked(false);

    _instances[handle] = base;

    klass->Create(*base);
    return Object(ObjectConstructParams(this, klass, handle));
}

void MarkObject(Object const &object, bool marked)
{
    if (!object.Exists())
        return;

    MarkObject(object.GetStorageBase(), marked);
}

void MarkObject(StorageBase &storage, bool marked)
{
    if (marked == storage.IsMarked())
        return;

    storage.SetMarked(marked);
    storage.GetClass()->SetMarked(storage, marked);
}

void MarkObjectAndChildren(Object const &object, bool marked)
{
    if (!object.Exists())
        return;

    MarkObjectAndChildren(object.GetStorageBase(), marked);
}

void MarkObjectAndChildren(StorageBase &storage, bool marked)
{
    MarkObject(storage, marked);
    for (auto const &[_, object] : storage.GetDictionary())
    {
        auto& child = const_cast<Object &>(object);
        if (child.GetHandle() == storage.GetHandle())    // HACK to sorta/kinda avoid cycles :/
            continue;

        MarkObjectAndChildren(child, marked);
    }
}

void Registry::GarbageCollect(Object root)
{ 
#ifdef KAI_USE_TRICOLOR
    AddRoot(root);
    TriColor();
#else
    MarkSweepAndDestroy(_root); 
#endif
}

void Registry::TriColor()
{
    // This is a magic number. the higher it is, the more objects may be deleted in this call
    // the cost has to be paid at some point, so this number really means "how much do I want
    // to spread out cost of GC over time versus memory use".
    //
    // Ideally, self-monitoring in real-time will adjust this number to avoid any pauses in the
    // realtime simulation
    //
    // if you have lots of memory, set max_cycles to 1. (or zero!). if not, set it higher
    // until you can fit memory usage into a sequence of frames.
    //
    // See also https://github.com/cschladetsch/Monotonic 
    const int MaxCycles = 17;

    if (gc_trace_level >= 1)
        KAI_TRACE_3(static_cast<int>(_instances.size()), static_cast<int>(_grey.size()), static_cast<int>(_white.size()));

    int cycle = 0;
    for (; cycle < MaxCycles; ++cycle)
    {
#ifdef KAI_DEBUG_REGISTRY
        if (gc_trace_level > 2) 
            TraceTriColor();
#endif

        if (_grey.empty())
        {
            ReleaseWhite();
            break;
        }

        auto iterator = _grey.begin();
        const Handle handle = *iterator;
        _grey.erase(iterator);
        StorageBase *base = GetStorageBase(handle);
        if (base == nullptr)
            continue;

        base->MakeReachableGrey();
        base->SetColor(ObjectColor::Black);
    }

    if (gc_trace_level >= 1)
        KAI_TRACE() << "TriColor: " << cycle << " passes";
}

void Registry::ReleaseWhite()
{
    // Make a copy of the _white set to avoid mutation while iterating.
    // Yes, this is expensive and is a good candidate to use Monotonic memory
    // allocation.
    std::vector<Handle> to_delete(_white.begin(), _white.end());
    _white.clear();

    for (auto const &handle : to_delete)
        DestroyObject(handle);
}

void Registry::SetGCTraceLevel(int N)
{
    gc_trace_level = N;
}

static void RemoveFromSet(Registry::ColoredSet &handles, Handle handle)
{
    Registry::ColoredSet::iterator entry = handles.find(handle);
    if (entry != handles.end())
        handles.erase(entry);
}

bool Registry::SetColor(StorageBase &base, ObjectColor::Color color)
{
    // When using TriColor, if an object is marked it means it has been forced deleted.
    if (base.IsMarked() && color != ObjectColor::White)
        return false;
    
#ifdef KAI_DEBUG_REGISTRY
    if (IsWatching(base) && gc_trace_level > 0)
        KAI_TRACE_3(base.GetHandle(), base.GetClass()->GetName(), color);
#endif

    Handle handle = base.GetHandle();
    switch (color)
    {
    case ObjectColor::White: 
        RemoveFromSet(_grey, handle);
        _white.insert(handle); 
        break;

    case ObjectColor::Grey: 
        RemoveFromSet(_white, handle);
        _grey.insert(handle); 
        break;

    case ObjectColor::Black:
        break;
    }

    return true;
}

void Registry::SetTree(Tree &tree)
{ 
    this->_tree = &tree;
    AddRoot(this->_tree->GetRoot());
}

template <class II, class T, class Pred>
II find(II A, II B, T X, Pred P)
{
    for (; A != B; ++A)
    {
        if (P(X, *A))
            break;
    }

    return A;
}

bool SameHandle(Object const &A, Object const &B)
{
    return A.GetHandle() == B.GetHandle();
}

void Registry::AddRoot(Object const &root)
{
    if (!root.Exists())
        return;

    if (find(_roots.begin(), _roots.end(), root, SameHandle) == _roots.end())
        _roots.push_back(root);

    SetColor(root.GetStorageBase(), ObjectColor::Grey);
}

#ifdef KAI_DEBUG_REGISTRY
String Registry::Trace() const
{
    return "";
}

void Registry::WatchObject(Handle handle, bool watch)
{
    if (watch)
    {
        observed.insert(handle);
        return;
    }

    Observed::iterator A = observed.find(handle);
    if (A != observed.end())
        observed.erase(A);
}

bool Registry::IsWatching(Handle handle) const
{
    StorageBase *base = GetStorageBase(handle);
    return IsWatchingObject(handle) || (base && IsWatchingType(base->GetClass()->GetTypeNumber()));
}

void Registry::WatchAllTypes()
{
    for (auto klass : _classes)
        if (klass)
            WatchType(klass->GetTypeNumber());
}

void Registry::WatchNoTypes()
{
    observed_types.clear();
}

void Registry::WatchType(Type::Number N, bool watch)
{
    if (watch)
    {
        observed_types.insert(N);
        return;
    }

    ObservedTypes::iterator A = observed_types.find(N);
    if (A != observed_types.end())
        observed_types.erase(A);
}

void Registry::TraceSet(Registry::ColoredSet const &set, const char *_name) const
{
    KAI_TRACE() << _name << "(" << (int)set.size() << "): ";
    if (gc_trace_level > 2)
    {
        for (auto handle : set)
        {
            Object Q = GetObject(handle);
            if (!Q.Exists())
                continue;
            KAI_TRACE() << Q.GetLabel() << "(" << Q.GetClass()->GetName() << "): " << handle.GetValue() << ", " << Q;
        }
    }
}

void Registry::TraceTriColor() const
{
    TraceGCCounts();
    if (gc_trace_level > 1)
        TraceWhite();

    if (gc_trace_level > 1)
        TraceGrey();
}

void Registry::TraceGCCounts() const
{
    KAI_TRACE_3((int)_instances.size(), (int)_grey.size(), (int)_white.size());
}

void Registry::TraceGrey() const
{
    TraceSet(_grey, "***GREY*** ");
}

void Registry::TraceWhite() const
{
    TraceSet(_white, "***WHITE*** ");
}

#endif

KAI_END

//EOF
