// (C) 2014 christian.schladetsch@gmail.com

#include "KAI/KAI.h"
#include "KAI/Algorithm.h"
#include "KAI/Tree.h"

#include "KAI/BuiltinTypes/Stack.h"

#ifdef WIN32
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	undef GetObject
#endif

#include <iostream>
#include <fstream>
#include <assert.h>

// use tri-color generational gc.
#define KAI_USE_TRICOLOR

KAI_BEGIN

Registry::Registry() 
	: allocator(new Memory::StandardAllocator())
	, owns_allocator(true)
{
	Construct();
}

Registry::Registry(Memory::IAllocator *alloc) 
	: allocator(alloc)
	, owns_allocator(false)
{
	Construct();
}

void Registry::Construct()
{
	classes.resize(2000, 0);

	gc_trace_level = 0;
	tree = 0;
	std::fill(classes.begin(), classes.end(), (ClassBase const *)0);
}

Registry::~Registry()
{
	Clear();

	if (owns_allocator)
		delete allocator;
}

void Registry::Clear()
{
	ClearInstances();
	ForEach(classes, Deleter<ClassBase>());
}

void Registry::ClearInstances()
{
	// create a set of handles to destroy, then destroy them
	// this can't be done in one pass, as otherwise we would be mutating the
	// container as we traverse it
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
	for (auto const &ins : instances)
		deathrow.insert(ins.first);
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
	Label classname = classname_str;
	const ClassBase *klass = GetClass(classname);
	if (klass == 0)
		KAI_THROW_1(UnknownClass<>, String(classname_str));

	return NewFromClass(klass);
}

const ClassBase *Registry::GetClass(const Label &name)
{
	//foreach (ClassBase const *klass, classes)
	for (auto const klass : classes)
		if (klass && klass->GetName() == name)
			return klass;

	return 0;
}

void Registry::DestroyNominated()
{
	// copy the elements in deathrow because when we delete
	// objects, they may release other objects.
	std::vector<Handle> dr(deathrow.begin(), deathrow.end());
	deathrow.clear();
	for (auto const &ob : dr)
		DestroyObject(ob);
}

void Registry::DestroyObject(Handle handle, bool force)
{
	bool succeeded = false;
	KAI_TRY
	{
		Instances::iterator iter = instances.find(handle);
		if (iter == instances.end())
		{
#ifdef KAI_DEBUG
			if (IsWatching(handle))
			{
				KAI_TRACE() << handle << ": doesn't exist, not deleted";
			}
#endif
			return;
		}

		StorageBase &base = *iter->second;
		assert(base.GetHandle() == handle);
		if (!base.IsManaged() && !force)
		{
#ifdef KAI_DEBUG
			if (IsWatching(handle))
				KAI_TRACE() << handle << ": " << base << " is not managed, not deleted";
#endif
			return;
		}

#ifdef KAI_DEBUG
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
		// when using tri-color GC, objects are always deleted when they are destroyed
		if (!base.GetClass()->Destroy(base) && !force)
		{
			base.SetColor(ObjectColor::Grey);
			return;
		}
#endif
		base.GetClass()->Delete(base);
		instances.erase(iter);

		RetainedObjects::iterator retained = retained_objects.find(handle);
		if (retained != retained_objects.end())
		{
			retained_objects.erase(retained);
		}

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
		KAI_TRACE() << "*** AWESOMELY BAD EXCEPTION deleting object ***";
		Instances::iterator iter = instances.find(handle);
		if (iter != instances.end())
		{
			// this leaks and has other *TERRIBLE* consequences but it is the best we can do to keep afloat
			instances.erase(iter);
		}
	}
}

void Registry::PruneRetained()
{
	RetainedObjects::iterator retained = retained_objects.begin(), end = retained_objects.end();
	while (retained != end)
	{
		Object object = GetObject(*retained);
		if (!object.Exists())
			retained = retained_objects.erase(retained);
		else
			++retained;
	}
}

const ClassBase *Registry::GetClass(Type::Number type_number)
{
	return classes[type_number.ToInt()];
}

StorageBase *Registry::GetStorageBase(Handle handle) const
{
	if (handle == 0)
		return 0;

	auto obj = instances.find(handle);
	if (obj == instances.end())
		return 0;

	return obj->second;
}

bool Registry::OnDeathRow(Handle handle) const
{
	return deathrow.find(handle) != deathrow.end();
}

void Registry::AddClass(const ClassBase *klass)
{
	if (klass == 0)
		KAI_THROW_0(NullObject);

	if (GetClass(klass->GetTypeNumber()))
		KAI_THROW_1(Base, "Duplicate Class");

	classes[klass->GetTypeNumber().ToInt()] = klass;
}

Object Registry::GetObject(Handle handle) const
{
#ifdef KAI_DEBUG
	if (IsWatching(handle) && gc_trace_level > 1)
		KAI_TRACE() << handle;
#endif

	if (handle == 0)
		return Object();

	Instances::const_iterator A = instances.find(handle);
	if (A == instances.end())
		return Object();

	return *A->second;
}

void Registry::MarkSweepAndDestroy(Object root)
{
#ifdef KAI_USE_TRICOLOR
	AddRoot(root);
	TriColor();
	return;
#else
	DestroyNominated();			// destroy any pending
	MarkAndSweep(root);			// mark objects for destruction
	DestroyNominated();			// destroy them
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
	// now mark everything that is reachable from the given object root
	if (root.Exists())
		MarkAll(root.GetStorageBase(), true);
}

void Registry::MarkAll(StorageBase &root, bool marked)
{
	MarkObject(root, marked);
	const Dictionary &dict = root.GetDictionary();
	Dictionary::const_iterator C = dict.begin(), D = dict.end();
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

	Instances::iterator A = instances.begin(), B = instances.end();
	// the handle of the next object created
	Handle last = next_handle.GetValue();
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

#ifdef KAI_DEBUG
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
#ifdef KAI_DEBUG
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

Object Registry::GetObjectFoo(Handle H) const
{
	return this->GetObject(H);
}

void Registry::GarbageCollect()
{
	if (!tree)
		return;
	GarbageCollect(tree->GetRoot());
}

Object Registry::NewFromClass(const ClassBase *klass)
{
	if (klass == 0)
		KAI_THROW_1(UnknownClass<>, "NULL Class");

	Handle handle = next_handle.NextValue();
	StorageBase *base = 0;
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
		Object &child = const_cast<Object &>(A->second);
		if (child.GetHandle() == Q.GetHandle())	// HACK to sorta/kinda avoid cycles :/
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
	// this is a magic number. the higher it is, the more objects may be deleted in this call
	// the cost has to be paid at some point, so this number really means "how much do I want
	// to spread out cost of GC over time versus memory use".
	//
	// if you have lots of memory, set max_cycles to 1. (or zero!). if not, set it higher
	// until you can fit memory usage into a sequence of frames.
	//
	// see also https://github.com/cschladetsch/Monotonic 
	const int max_cycles = 17;
	if (gc_trace_level >= 1)
	{
		KAI_TRACE_3(instances.size(), grey.size(), white.size());
	}

	int cycle = 0;
	for (; cycle < max_cycles; ++cycle)
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

		ColoredSet::iterator iter = grey.begin();
		Handle handle = *iter;
		grey.erase(iter);
		StorageBase *base = GetStorageBase(handle);
		if (base == 0)
			continue;

		base->MakeReachableGrey();
		base->SetColor(ObjectColor::Black);
	}

	if (gc_trace_level >= 1)
	{	
		KAI_TRACE() << "TriColor: " << cycle << " passes";
		printf("Tricolor passes\n");
	}
}

void Registry::ReleaseWhite()
{
	// make a copy of the white set to avoid mutation while iterating
	std::vector<Handle> to_delete(white.begin(), white.end());
	white.clear();
	//foreach (Handle handle, to_delete)
	for (auto const &handle : to_delete)
	{
		DestroyObject(handle);
	}
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
	// when using TriColor, if an object is marked it means it has been forced deleted
	if (base.IsMarked() && color != ObjectColor::White)
		return false;
	
#ifdef KAI_DEBUG_REGISTRY
	if (IsWatching(base) && gc_trace_level > 0)
	{
		KAI_TRACE_3(base.GetHandle(), base.GetClass()->GetName(), color);
	}
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
	if (!root)
		return;

	if (find(roots.begin(), roots.end(), root, SameHandle) == roots.end())
		roots.push_back(root);

	SetColor(root.GetStorageBase(), ObjectColor::Grey);
}

#ifdef KAI_DEBUG
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

void Registry::TraceSet(Registry::ColoredSet const &set, const char *name) const
{
	KAI_TRACE() << name << "(" << (int)set.size() << "): ";
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
	KAI_TRACE_3(instances.size(), grey.size(), white.size());
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
