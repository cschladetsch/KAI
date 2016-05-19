#pragma once

#include <KAI/Core/Config/Base.h>

#include <unordered_map>
#include <vector>
#include <set>
#include <list>

#include <KAI/Core/Memory/IAllocator.h>
#include <KAI/Core/Type/Traits.h>
#include "Pointer.h"

KAI_BEGIN

#ifdef KAI_DEBUG
#	ifndef KAI_DEBUG_REGISTRY
#		define KAI_DEBUG_REGISTRY
#	endif
#endif

class Tree;

// Use hash-tables for class and instance lookups
//#define KAI_HASH_TABLE_REGISTRY
#define KAI_BOOST_UNORDERED_REGISTRY

template <class T>
Pointer<ClassBase const *> NewClass(Registry &R, const Label &name);

class Registry
{
public:
#ifdef KAI_BOOST_UNORDERED_REGISTRY
	typedef std::unordered_map<Handle, StorageBase *, HashHandle> Instances;
#elif defined(KAI_HASH_TABLE_REGISTRY)
	typedef std::hash_map<Handle, StorageBase *, HashHandle> Instances;
#else
	typedef std::map<Handle, StorageBase *> Instances;
#endif
	typedef std::vector<const ClassBase *> Classes;
	typedef HandleSet Handles, Deathrow;
	typedef HandleSet RetainedObjects;
	typedef std::vector<Handle> VectorHandles;
	typedef std::set<Handle> ColoredSet;

	// pools of objects indexed by type number
	typedef std::vector<std::vector<StorageBase *> > Pools;

private:
	friend class StorageBase;
	ColoredSet white;	// condemned set. may be referenced by objects in the white or grey sets
	ColoredSet grey;	// may or may not have refernces to objects in the white, black or grey sets
	ColoredSet black;	// has no references to objects in the white set

	Classes classes;
	Instances instances;
	Handles retained;
	Deathrow deathrow;
	Handle next_handle;
	Tree *tree;
	RetainedObjects retained_objects;
	Pools pools;
	std::shared_ptr<Memory::IAllocator> allocator;
	bool owns_allocator;

public:
	Registry();
	Registry(std::shared_ptr<Memory::IAllocator>);
	~Registry();

	Memory::IAllocator &GetMemorySystem() const
	{
		return *allocator;
	}

	/// destroys all instances and types
	void Clear();

	/// destroys all instances
	void ClearInstances();

	const Instances &GetInstances() const { return instances; }
	const Classes &GetClasses() const { return classes; }

	template <class T>
	Pointer<ClassBase const *> AddClass()
	{
		return NewClass<T>(*this, Label(String(Type::Traits<T>::Name())));
	}
	template <class T>
	Pointer<ClassBase const *> AddClass(const Label &N)
	{
		return NewClass<T>(*this, N);
	}

	Pointer<ClassBase const *> AddClass(Type::Number, ClassBase const *);

	/// create a new instance of a given known type
	template <class T>
	Object New()
	{
		const ClassBase *klass = GetClass(Type::Traits<T>::Number);
		if (klass == 0)
			KAI_THROW_0(UnknownClass<T>);
		return NewFromClass(klass);
	}

	/// A number in range [0..100)
	typedef int Percentage;

	Percentage CalcMemoryUsage() const;
	Percentage CalcMemoryFragmentationPercentage() const;

	void DefragmentMemory();

	Tree *GetTree() const { return tree; }
	void SetTree(Tree &);

	// A pinned Object will not be moved in memory
	bool Pin(Handle);
	bool Unpin(Handle);

	bool Pin(const Object &Q) { return Pin(Q.GetHandle()); }
	bool Unpin(const Object &Q) { return Unpin(Q.GetHandle()); }

	template <class T>
	Object New(const T &X)
	{
		Pointer<T> P = New<T>();
		*P = X;
		return P;
	}

	//template <>
	Object New(const Object &Q)
	{
		return Q;
	}

	/// Retained process objects
	template <class T>
	Object NewRetained()
	{
		Object retained = New<T>();
		retained.SetManaged(false);
		retained.SetBlack();
		retained_objects.insert(retained.GetHandle());
		return retained;
	}
	void UpdateProcesses(float time_delta);
	RetainedObjects const &GetRetainedObjects() const { return retained_objects; }
	void PruneRetained();
	void DeleteRetained();

	template <class T>
	Storage<T> *NewStorage()
	{
		const ClassBase *klass = GetClass(Type::Traits<T>::Number);
		if (klass == 0)
			KAI_THROW_0(UnknownClass<T>);
		Object Q = NewFromClass(klass);
		return static_cast<Storage<T> *>(Q.GetBasePtr());
	}

	template <class T>
	const ClassBase *GetClass()
	{
		return GetClass(Type::Traits<T>::Number);
	}

	const ClassBase *GetClass(Type::Number);
	const ClassBase *GetClass(const Label &);
	StorageBase *GetStorageBase(Handle) const;

	Object GetObject(Handle) const;
	Object GetObjectFoo(Handle) const;

	template <class T>
	Pointer<T> GetPointer(Handle handle) const
	{
		return GetObject(handle);
	}

	template <class T>
	Pointer<const T> GetConstPointer(Handle handle) const
	{
		return GetObject(handle);
	}

	/// destroy an object given its handle
	void Delete(Handle);
	void Delete(Object const &);

	bool OnDeathRow(Handle) const;
	void AddClass(const ClassBase *K);

	Object NewFromTypeNumber(Type::Number type_number);
	Object NewFromClassName(const char *classname_str);
	Object NewFromClass(const ClassBase *klass);

	void NominateAll();

	template <class T>
	void FreeResources(T *p)
	{
		allocator->DeAllocate(p);
	}

	// delete all objects on deathrow, returning the number deleted
	void DestroyNominated();

	int GetDeathrowSize() const { return (int)deathrow.size(); }

	template <class T>
	Storage<T> *CloneStorage(const Object &Q)
	{
		KAI_NOT_IMPLEMENTED();
	}

	typedef std::list<Object> Roots;
	Roots roots;

	void AddRoot(Object const &root);

	void TriColor();
	void ReleaseWhite();

	void Mark(Object root);
	void Sweep();
	void MarkAndSweep(Object root);
	void MarkSweepAndDestroy(Object root);
	void GarbageCollect(Object root);
	void MarkAll(StorageBase &root, bool marked);

	void DestroyObject(Handle, bool force = false);
	void GarbageCollect();

	bool SetColor(StorageBase &, ObjectColor::Color);

	/// @group debugging
	int gc_trace_level;
	void SetGCTraceLevel(int);

#ifdef KAI_DEBUG_REGISTRY
	typedef std::set<Type::Number> ObservedTypes;
	typedef std::set<Handle> Observed;
	ObservedTypes observed_types;
	Observed observed;

	bool IsWatching(Object const &Q) const { return IsWatching(Q.GetHandle()); }
	bool IsWatching(Handle) const;

	void WatchObject(Handle handle, bool = true);
	void WatchObject(Object const &Q, bool W = true) { WatchObject(Q.GetHandle(), W); }
	void WatchAllObjects();
	void WatchNoObjects() { observed.clear(); }
	void StopWatchingObject(Handle handle) { WatchObject(handle, false); }
	bool IsWatchingObject(Object const &object) const { return IsWatchingObject(object.GetHandle()); }
	bool IsWatchingObject(Handle handle) const { return observed.find(handle) != observed.end(); }

	void WatchAllTypes();
	void WatchNoTypes();
	template <class T>
	void WatchType(bool watch = true) { WatchType(Type::Traits<T>::Number, watch); }
	void WatchType(Type::Number N, bool = true);
	void StopWatchingType(Type::Number N) { WatchType(N, false); }
	bool IsWatchingType(Type::Number N) const { return observed_types.find(N) != observed_types.end(); }

	void TraceGCCounts() const;
	void TraceGrey() const;
	void TraceWhite() const;

	void TraceSet(Registry::ColoredSet const &set, const char *name) const;
	void TraceTriColor() const;
	/// @endgroup debugging
	String Trace() const;
#endif

private:
	void Construct();
};

template <class T>
Storage<T> *NewStorage(Registry &R)
{
	return R.NewStorage<T>();
}

#	ifdef KAI_DEBUG_REGISTRY
//#		define KAI_REGISTRY_TRACE(Q) \
//			do { \
//				if (!Q.GetRegistry()) \
//				{ \
//					KAI_TRACE() << "Empty registry"; \
//				} \
//				else \
//				{ \
//					Q.GetRegistry().TraceObject(Q)
#		define KAI_DEBUG_WATCH_OBJECT(object) \
			if (object) object.GetRegistry()->WatchObject(object)
#	else
#		define KAI_DEBUG_WATCH_OBJECT(object) KAI_UNUSED_1(object)
#	endif

KAI_END

