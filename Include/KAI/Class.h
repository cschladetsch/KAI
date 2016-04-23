
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_CLASS_H
#	define KAI_CLASS_H

KAI_BEGIN

#pragma warning(push)

// warning C4702: unreachable code
#pragma warning(disable:4702)

void MarkGrey(Object const &);

/// A Class<T> manages instances of T
template <class T>
struct Class : ClassBase
{
	typedef Type::Traits<T> Traits;

	Class(Label const &name) : ClassBase(name, Type::Traits<T>::Number) { }

	int GetTraitsProperties() const
	{
		return Traits::Properties;
	}

	bool HasTraitsProperty(int N) const
	{
		return (Traits::Properties & N) != 0;
	}

	// Lifetime management
	StorageBase *NewStorage(Registry *registry, Handle handle) const 
	{
		return registry->GetMemorySystem().Allocate<Storage<T> >(ObjectConstructParams(registry, this, handle));
		//return Allocate<Storage<T> >(ObjectConstructParams(registry, this, handle));
	}

	void Create(StorageBase &storage) const
	{
		CreateProperties(storage);
		Traits::LifetimeManager::Create(TypedStorage(storage));
	}
	bool Destroy(StorageBase &storage) const
	{
		return Traits::LifetimeManager::Destroy(TypedStorage(storage));
	}

	void Delete(StorageBase &storage) const
	{
		if (!properties.empty())
		{
			foreach (Properties::value_type const &X, properties)
			{
				PropertyBase const &prop = *X.second;
				if (!prop.IsSystemType())
					continue;
				Object K = prop.GetObject(storage);
				if (K)
				{
					K.RemovedFromContainer(storage);
				}
			}
		}
		Traits::LifetimeManager::Delete(TypedStorage(storage));
	}
	Storage<T> *TypedStorage(StorageBase &storage) const
	{
		return reinterpret_cast<Storage<T> *>(&storage);
	}
	void CreateProperties(StorageBase &object) const 
	{
		foreach (Properties::value_type X, properties)
		{
			PropertyBase const &prop = *X.second;
			if (!prop.IsSystemType())
				continue;
			if (!prop.CreateDefaultValue())
				continue;
			Object value = NewFromTypeNumber(prop.GetFieldTypeNumber());
			prop.SetObject(object, value);
		}
	}

	void Assign(StorageBase &A, StorageBase const &B) const
	{
		Traits::Assign::Perform(Deref<T>(A), ConstDeref<T>(B));
	}
	Object Duplicate(StorageBase const &parent) const
	{
		Storage<T> *result = ClassBase::NewStorage<T>();
		Traits::Assign::Perform(result->GetReference(), ConstDeref<T>(parent));
		foreach (Properties::value_type const &property, properties)
		{
			PropertyBase const &prop = *property.second;
			// if it is a system-type property, clone it, else just store the value
			if (prop.IsSystemType())
			{
				auto ch = prop.GetObject(parent);
				if (ch.Exists())
					prop.SetObject(*result, ch.Clone());
				else
					prop.SetObject(*result, Object());
			}
			else
				prop.SetValue(*result, prop.GetValue(parent));
		}
		return result;
	}

	/// @group container operations

	void GetContainedObjects(StorageBase &object, ObjectList &contained) const
	{
		Traits::ContainerOps::ForEachContained(CleanDeref<T>(object), AddContainedFun<T, ObjectList>(contained));
	}

	void SetReferencedObjectsColor(StorageBase &Q, ObjectColor::Color C, StorageBase::Handles &H) const
	{
		ClassBase::SetReferencedObjectsColor(Q, C, H);
		Traits::ContainerOps::ForEachContained(CleanDeref<T>(Q), SetObjectColorRecursive<T>(C, H));
	}

	void SetSwitch(StorageBase &Q, int S, bool M) const
	{
		Traits::ContainerOps::SetSwitch(CleanDeref<T>(Q), S, M);
	}

	void DetachFromContainer(StorageBase &Q, Object const &K) const
	{
		Traits::ContainerOps::Erase(Deref<T>(Q), K);
	}

	void SetMarked2(StorageBase &Q, bool M) const
	{
		Traits::ContainerOps::SetMarked(CleanDeref<T>(Q), M);
	}
	void MakeReachableGrey(StorageBase &base) const
	{
		ClassBase::MakeReachableGrey(base);
		Traits::ContainerOps::ForEachContained(CleanDeref<T>(base), MakeReachableGreyFun<T>());
	}
	/// @endgroup container operations

	Object UpCast(StorageBase &Q) const
	{
		StorageBase *base = Traits::UpCaster::Cast(*Q.GetRegistry(), Deref<T>(Q));
		if (base == 0)
			KAI_THROW_0(BadUpCast);
		return Object(ObjectConstructParams(base->GetRegistry(), base->GetClass(), base->GetHandle()));
	}

	Object CrossCast(StorageBase &, Type::Number) const
	{
		KAI_NOT_IMPLEMENTED();
	}

	Object DownCast(StorageBase &, Type::Number) const
	{
		KAI_NOT_IMPLEMENTED();
	}

	HashValue GetHashValue(const StorageBase &Q) const
	{
		return Traits::HashFunction::Calc(ConstDeref<T>(Q));
	}

	Object Absolute(const StorageBase &object) const
	{
		Object result = object.Clone();
		Traits::Absolute::Perform(Deref<T>(object));
		return result;
	}
	bool Less(const StorageBase &A, const StorageBase &B) const
	{
		return Traits::Less::Perform(ConstDeref<T>(A), ConstDeref<T>(B));
	}

	bool Equiv(const StorageBase &A, const StorageBase &B) const
	{
		if (!HasTraitsProperty(Type::Properties::Equiv) && HasTraitsProperty(Type::Properties::Less))
			return !Less(A,B) && !Less(B,A);
		return Traits::Equiv::Perform(ConstDeref<T>(A),ConstDeref<T>(B));
	}
	bool Greater(const StorageBase &A, const StorageBase &B) const
	{
		return Traits::Greater::Perform(ConstDeref<T>(A), ConstDeref<T>(B));
	}
	StorageBase *Plus(StorageBase const &A, StorageBase const &B) const
	{
		Storage<T> *R = ClassBase::NewStorage<T>();
		Traits::Assign::Perform(R->GetReference(), Traits::Plus::Perform(ConstDeref<T>(A), ConstDeref<T>(B)));
		return R;
	}
	StorageBase *Minus(StorageBase const &A, StorageBase const &B) const
	{
		Storage<T> *R = ClassBase::NewStorage<T>();
		Traits::Assign::Perform(R->GetReference(), Traits::Minus::Perform(ConstDeref<T>(A), ConstDeref<T>(B)));
		return R;
	}
	StorageBase *Multiply(StorageBase const &A, StorageBase const &B) const
	{
		Storage<T> *R = ClassBase::NewStorage<T>();
		Traits::Assign::Perform(R->GetReference(), Traits::Multiply::Perform(ConstDeref<T>(A), ConstDeref<T>(B)));
		return R;
	}
	StorageBase *Divide(StorageBase const &A, StorageBase const &B) const
	{
		Storage<T> *R = ClassBase::NewStorage<T>();
		Traits::Assign::Perform(R->GetReference(), Traits::Divide::Perform(ConstDeref<T>(A), ConstDeref<T>(B)));
		return R;
	}

	void Insert(StringStream &S, const StorageBase &Q) const
	{
		Traits::StringStreamInsert::Insert(S, ConstDeref<T>(Q));
	}

	StorageBase *Extract(Registry &R, StringStream &S) const
	{
		Storage<T> *Q = ClassBase::NewStorage<T>(R);
		Traits::StringStreamExtract::Extract(S, Q->GetReference());
		return Q;
	}

	void ExtractValue(Object &object, StringStream &strstream) const
	{
		Traits::StringStreamExtract::Extract(strstream, Deref<T>(object));
	}

	void Insert(BinaryStream &S, const StorageBase &Q) const
	{
		Traits::BinaryStreamInsert::Insert(S, ConstDeref<T>(Q));
	}
	StorageBase *Extract(Registry &R, BinaryPacket &S) const
	{
		Storage<T> *Q = ClassBase::NewStorage<T>(R);
		Traits::BinaryPacketExtract::Extract(S, Q->GetReference());
		return Q;
	}

};

#	pragma warning(pop)

KAI_END

#endif // KAI_CLASS_H

//EOF
