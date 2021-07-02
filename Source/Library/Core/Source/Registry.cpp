#include <KAI/Core/Registry.h>
#include <KAI/Core/BuiltinTypes/Signed32.h>
#include <KAI/Core/Memory/StandardAllocator.h>
#include <KAI/Core/Object/Class.h>
#include <KAI/Core/Object/IObject.h>
#include "KAI/Core/Tree.h"
#include "KAI/Core/TriColor.h"

// Use tri-color generational gc. See https://en.wikipedia.org/wiki/Tracing_garbage_collection#Tri-color_marking
#define KAI_USE_TRICOLOR

KAI_BEGIN

Registry::Registry()
{
    allocator = std::make_shared<Memory::StandardAllocator>();
    Construct();
}

Registry::Registry(std::shared_ptr<Memory::IAllocator> alloc)
{
    allocator = alloc;
    Construct();
}

void Registry::Construct()
{
    classes.resize(2000, nullptr);
    gc_trace_level = 1;
    tree = nullptr;
    std::fill(classes.begin(), classes.end(), (ClassBase const *)nullptr);
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
    for (auto const &instance : instances)
        handles.push_back(instance.first);

    for (auto const &handle : handles)
        DestroyObject(handle, true);

    instances.clear();
}

void Registry::NominateAll()
{
    deathrow.clear();
    for (const auto & [handle, _] : instances)
        deathrow.insert(handle);
}

Object Registry::NewFromTypeNumber(Type::Number type_number)
{
    ClassBase const *klass = classes[type_number.ToInt()];
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
    for (auto const klass : classes)
        if (klass && klass->GetName() == name)
            return klass;

    return nullptr;
}

void Registry::DestroyNominated()
{
    // Copy the elements in deat row because when we delete
    // objects, they may release other objects.
    std::vector dr(deathrow.begin(), deathrow.end());
    deathrow.clear();
    for (auto const &ob : dr)
        DestroyObject(ob);
}

void Registry::DestroyObject(Handle handle, bool force)
{
    bool succeeded = false;
    KAI_TRY
    {
        const auto found = instances.find(handle);
        if (found == instances.end())
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
        instances.erase(found);

        RetainedObjects::iterator retained = retained_objects.find(handle);
        if (retained != retained_objects.end())
            retained_objects.erase(retained);

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
        auto const iter = instances.find(handle);
        if (iter != instances.end())
        {
            // this leaks and has other *TERRIBLE* consequences but it is the best we can do to keep afloat
            instances.erase(iter);
        }
    }
}

void Registry::PruneRetained()
{
    auto retained = retained_objects.begin();
    const auto end = retained_objects.end();
    while (retained != end)
    {
        if (Object object = GetObject(*retained); !object.Exists())
            retained = retained_objects.erase(retained);
        else
            ++retained;
    }
}

const ClassBase *Registry::GetClass(Type::Number type_number)
{
    const auto tn = type_number.ToInt();
    if (tn >= static_cast<int>(classes.size()))
        KAI_THROW_1(LogicError, "Inalid type number");

    return classes[tn];
}

StorageBase *Registry::GetStorageBase(Handle handle) const
{
    if (handle == Handle(0))
        return nullptr;

    const auto obj = instances.find(handle);
    if (obj == instances.end())
        return nullptr;

    return obj->second;
}

bool Registry::OnDeathRow(Handle handle) const
{
    return deathrow.find(handle) != deathrow.end();
}

void Registry::AddClass(const ClassBase *klass)
{
    if (klass == nullptr)
        KAI_THROW_0(NullObject);

    if (GetClass(klass->GetTypeNumber()))
        KAI_THROW_1(Base, "Duplicate Class");

    classes[klass->GetTypeNumber().ToInt()] = klass;
}

Object Registry::GetObject(Handle handle) const
{
#ifdef KAI_DEBUG_REGISTRY
    if (IsWatching(handle) && gc_trace_level > 1)
        KAI_TRACE() << handle;
#endif

    if (handle == Handle(0))
        return Object();

    const auto found = instances.find(handle);
    if (found == instances.end())
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
    MarkAndSweep(root);            // mark objects for destruction
    DestroyNominated();            // destroy them
#endif
}

void Registry::MarkAndSweep(Object root)
{
    Mark(root);
    Sweep();
}

void MarkAll(StorageBase &root, bool marked);

void Registry::Mark(Object root)
{
    // Mark everything that is reachable from the given object root.
    if (root.Exists())
        MarkAll(root.GetStorageBase(), true);
}

void Registry::MarkAll(StorageBase &root, bool marked)
{
    MarkObject(root, marked);
    const Dictionary &dict = root.GetDictionary();
    auto C = dict.begin(), D = dict.end();
    for (; C != D; ++C)
    {
        Object const &child = C->second;
        StorageBase *base = GetStorageBase(child.GetHandle());
        if (base == 0)
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
    if (instances.empty())
        return;

    auto A = instances.begin(), B = instances.end();
    // the handle of the next object created
    auto last = Handle(next_handle.GetValue());
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
    // use Object::Delete
    (void)handle;
    throw;
#else

#ifdef KAI_DEBUG_REGISTRY
    if (IsObserving(handle))
    {
        KAI_TRACE() << handle;
    }
#endif
    // if unknown handle, do nothing
    Instances::const_iterator instance = instances.find(handle);
    if (instance == instances.end())
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

    // remove from list of retained objects
    auto etained = retained_objects.find(handle);
    if (retained != retained_objects.end())
        retained_objects.erase(retained);

    // mark for pending collection
#ifndef KAI_USE_TRICOLOR
    deathrow.insert(handle);
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
    classes[N.ToInt()] = K;
    return Pointer<ClassBase const *>();
}

void Registry::GarbageCollect()
{
    if (!tree)
        return;

    GarbageCollect(tree->GetRoot());
}

Object Registry::NewFromClass(const ClassBase *klass)
{
    if (klass == nullptr)
        KAI_THROW_1(UnknownClass<>, "NULL Class");

    Handle handle(next_handle.NextValue());
    StorageBase *base = nullptr;
    base = klass->NewStorage(this, handle);

#ifdef KAI_DEBUG_REGISTRY
    if (IsWatchingType(klass->GetTypeNumber()))
        KAI_TRACE() << klass->GetName() << ": " << handle;
#endif

    base->SetColor(ObjectColor::White);
    base->SetMarked(false);

    instances[handle] = base;

    klass->Create(*base);
    return Object(ObjectConstructParams(this, klass, handle));
}

void MarkObject(Object const &Q, bool M)
{
    if (!Q.Exists())
        return;

    MarkObject(Q.GetStorageBase(), M);
}

void MarkObject(StorageBase &Q, bool M)
{
    if (M == Q.IsMarked())
        return;

    Q.SetMarked(M);
    Q.GetClass()->SetMarked(Q, M);
}

void MarkObjectAndChildren(Object const &Q, bool M)
{
    if (!Q.Exists())
        return;

    MarkObjectAndChildren(Q.GetStorageBase(), M);
}

void MarkObjectAndChildren(StorageBase &Q, bool M)
{
    MarkObject(Q, M);
    Dictionary::const_iterator A = Q.GetDictionary().begin(), B = Q.GetDictionary().end();
    for (; A != B; ++A)
    {
        auto& child = const_cast<Object &>(A->second);
        if (child.GetHandle() == Q.GetHandle())    // HACK to sorta/kinda avoid cycles :/
            continue;

        MarkObjectAndChildren(child, M);
    }
}

void Registry::GarbageCollect(Object root)
{ 
#ifdef KAI_USE_TRICOLOR
    AddRoot(root);
    TriColor();
#else
    MarkSweepAndDestroy(root); 
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
        KAI_TRACE_3(static_cast<int>(instances.size()), static_cast<int>(grey.size()), static_cast<int>(white.size()));

    int cycle = 0;
    for (; cycle < MaxCycles; ++cycle)
    {
#ifdef KAI_DEBUG_REGISTRY
        if (gc_trace_level > 2) 
            TraceTriColor();
#endif

        if (grey.empty())
        {
            ReleaseWhite();
            break;
        }

        auto iterator = grey.begin();
        const Handle handle = *iterator;
        grey.erase(iterator);
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
    // Make a copy of the white set to avoid mutation while iterating.
    // Yes, this is expensive and is a good candidate to use Monotonic memory
    // allocation.
    std::vector<Handle> to_delete(white.begin(), white.end());
    white.clear();

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
        RemoveFromSet(grey, handle);
        white.insert(handle); 
        break;

    case ObjectColor::Grey: 
        RemoveFromSet(white, handle);
        grey.insert(handle); 
        break;

    case ObjectColor::Black:
        break;
    }

    return true;
}

void Registry::SetTree(Tree &tree)
{ 
    this->tree = &tree;
    AddRoot(this->tree->GetRoot());
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

    if (find(roots.begin(), roots.end(), root, SameHandle) == roots.end())
        roots.push_back(root);

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
    for (auto klass : classes)
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
    KAI_TRACE_3((int)instances.size(), (int)grey.size(), (int)white.size());
}

void Registry::TraceGrey() const
{
    TraceSet(grey, "***GREY*** ");
}

void Registry::TraceWhite() const
{
    TraceSet(white, "***WHITE*** ");
}

#endif

KAI_END

//EOF
