#pragma once

#include <list>

#include "KAI/Core/Base.h"
#include "KAI/Core/ObjectColor.h"
#include "KAI/Core/Object/Handle.h"
#include "KAI/Core/Object/Constness.h"
#include "KAI/Core/Object/ObjectConstructParams.h"
#include <KAI/Core/BuiltinTypes/Dictionary.h>
#include <KAI/Core/Type.h>

KAI_BEGIN

class Object
{
private:
	const ClassBase *class_base;
	Registry *registry;
	Handle handle;

#ifdef KAI_CACHE_OBJECT_LOOKUPS
	// these fields are used to cache results for speed
	int gcindex;
	bool valid;
	void *value;
#endif

public:
	enum Switch 
	{ 
		Marked = 1, 
		Managed = 2, 
		Const = 4, 
		Clean = 8, 
		// if set, when marking, will not mark children
		NoRecurse = 16,
		DefaultSwitches = Managed
	};

	Object() : registry(0), class_base(0) { }
	Object(Object const &);
	Object(ObjectConstructParams const &P);

	Object &operator=(Object const &);

	StorageBase &GetStorageBase() const;
	int GetSwitches() const;

	ObjectColor::Color GetColor() const;
	void SetColor(ObjectColor::Color C) const;
	void SetColorRecursive(ObjectColor::Color C) const;
	void SetColorRecursive(ObjectColor::Color C, HandleSet &) const;
	bool IsWhite() const { return GetColor() == ObjectColor::White; }
	bool IsGrey() const { return GetColor() == ObjectColor::Grey; }
	bool IsBlack() const { return GetColor() == ObjectColor::Black; }
	void SetWhite() const { SetColor(ObjectColor::White); }
	void SetGrey() const { SetColor(ObjectColor::Grey); }
	void SetBlack() const { SetColor(ObjectColor::Black); }

	Type::Number GetTypeNumber() const;
	const ClassBase *GetClass() const { return class_base; }
	Registry *GetRegistry() const { return registry; }

	Object Duplicate() const;
	Object Clone() const { return Duplicate(); }

	bool IsTypeNumber(int N) const 
	{ 
		if (!Exists())
			return N == Type::Number::None;

		return GetTypeNumber() == N;
	}

	Handle GetParentHandle() const;
	void SetParentHandle(Handle);
	Handle GetHandle() const { return handle; }
	Object GetParent() const;

	template <class T>
	bool IsType() const { return Exists() && GetTypeNumber() == Type::Traits<T>::Number; }

	void Delete() const;
	bool Valid() const;
	bool Exists() const;
	bool OnDeathRow() const;

	bool IsConst() const;
	bool IsManaged() const;
	bool IsMarked() const;
	bool IsClean() const;

	void SetSwitch(int, bool) const;
	void SetSwitches(int) const;

	void SetMarked(bool = true) const;
	void SetConst() const;
	void SetManaged(bool = true) const;
	void SetClean(bool = true) const;

	bool IsMutable() const { return !IsConst(); }
	bool IsUnmanaged() const { return !IsManaged(); }
	bool IsUnmarked() const { return !IsMarked(); }
	bool IsDirty() const { return !IsClean(); }

	// use these to get/set/remove a property or a child of a given name
	void Set(const char *N, const Object &Q) const { Set(Label(N), Q); }
	Object Get(const char *N) const { return Get(Label(N)); }

	void Set(const Label &, const Object &) const;
	Object Get(const Label &) const;

	bool Has(const Label &) const;
	void Remove(const Label &) const;

	template <class T>
	void SetValue(const Label &L, const T &V) const;

	template <class T>
	T const &GetValue(const Label &L) const;

	template <class T>
	T &GetValue(const Label &L);
	void Detach(const Label &L) const { Remove(L); }
	void Detach(const Object &Q) const;
	Dictionary const &GetDictionary() const;
	void SetChild(const Label &L, const Object &Q) const { Set(L, Q); }
	Object GetChild(const Label &L) const { return Get(L); }
	void RemoveChild(const Label &L) const { Remove(L); }
	void DetachChild(const Label &L) const { Remove(L); }
	void DetachChild(const Object &Q) const { Detach(Q); }
	bool HasChild(const Label &L) const { return Has(L); }

	Label GetLabel() const;

	String ToString() const;
	String ToXmlString() const;

	// deref's Registry so must be in source file
	Object NewFromTypeNumber(Type::Number N) const;
	Object NewFromClassName(String const &type_name) const;

	void Assign(StorageBase &, StorageBase const &);

	/// return the storage of the given other object within the registry that made this object
	StorageBase *GetStorageBase(Handle other) const;
	void SetPropertyValue(Label const &, Object const &) const;
	Object GetPropertyValue(Label const &) const;

	void SetPropertyObject(Label const &, Object const &) const;
	Object GetPropertyObject(Label const &) const;

	bool HasProperty(Label const &name) const;


	/// detach from parent
	void Detach();

	static void Register(Registry &);

	/// detach from container
	void RemovedFromContainer(Object container) const;
	
	/// attach to container
	void AddedToContainer(Object container) const;

	StorageBase *GetBasePtr() const;
	StorageBase *GetParentBasePtr() const;

	typedef std::list<Object> ObjectList;
	void GetPropertyObjects(ObjectList &contained) const;
	void GetContainedObjects(ObjectList &contained) const;
	void GetChildObjects(ObjectList &contained) const;
	void GetAllReferencedObjects(ObjectList &contained) const;

	class ChildProxy
	{
		friend class Object;
		Registry *registry;
		Handle handle;
		Label label;
		Constness konst;
		ChildProxy(Object const &Q, const char *);
		ChildProxy(Object const &Q, Label const &L);
		Object GetObject() const;
	public:
		template <class T>
		ChildProxy &operator=(T const &value)
		{
			GetObject().SetValue(label, value);
			return *this;
		}

		template <class T>
		ChildProxy &operator=(Pointer<T> const &value)
		{
			GetObject().Set(label, value);
			return *this;
		}
		ChildProxy &operator=(Object const &child)
		{
			GetObject().Set(label, child);
			return *this;
		}
		operator Object() const
		{
			return GetObject().Get(label);
		}
	};

	ChildProxy operator[](const char *L)
	{
		return ChildProxy(*this, L);
	}
	ChildProxy operator[](Label const &L)
	{
		return ChildProxy(*this, L);
	}

protected:
	Dictionary &GetDictionaryRef();
};

StringStream &operator<<(StringStream &S, const Object &Q);
StringStream &operator>>(StringStream &S, Object &Q);
BinaryStream &operator<<(BinaryStream &S, const Object &Q);
BinaryStream &operator>>(BinaryStream &stream, Object &Q);

Object operator*(Object const &A);
bool operator<(Object const &A, Object const &B);
bool operator==(Object const &A, Object const &B);
inline bool operator!=(Object const &A, Object const &B) { return !(A == B); }
bool operator>(Object const &A, Object const &B);

Object operator+(Object const &A, Object const &B);
// WTF Object operator-(Object const &Object Absolute(Object const &A);

KAI_TYPE_TRAITS(Object, Number::Object
	, Properties::StringStreamInsert
	| Properties::BinaryStreamInsert
	| Properties::BinaryStreamExtract);

HashValue GetHash(Object const  &);

void MarkObject(Object const &, bool = true);
void MarkObjectAndChildren(Object const &, bool = true);

void MarkObject(StorageBase &, bool = true);
void MarkObjectAndChildren(StorageBase &, bool = true);

Object Duplicate(Object const &);

KAI_END

namespace boost
{
	inline size_t hash_value(KAI_NAMESPACE(Object) const &H)
	{
		return H.GetHandle().GetValue();
	}
}
