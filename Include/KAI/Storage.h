#pragma once

KAI_BEGIN

template <class T>
class ConstStorage : public StorageBase//, IConstStorage<T>
{
	typedef typename Type::Traits<T> Tr;
	typedef typename Tr::Store Store;

protected:
	Store stored;

public:
	ConstStorage(const ObjectConstructParams &P) : StorageBase(P) { SetClean(); }

	typename Tr::ConstReference GetConstReference() const 
	{ 
		return stored; 
	}
	typename Tr::ConstReference operator*() const 
	{ 
		return stored; 
	}
	typename Tr::ConstPointer operator->() const 
	{ 
		return &stored; 
	}
};

template <class T>
class ConstStorage<Container<T> > : public StorageBase//, IConstStorage<T>
{
	typedef typename Type::Traits<T> Tr;
	typedef typename Tr::Store Stored;

protected:
	Stored stored;

public:
	ConstStorage(const ObjectConstructParams &P) : StorageBase(P), stored(5/*P.registry->GetAllocator().GetHeap()*/) 
	{ 
		SetClean(); 
	}

	typename Tr::ConstReference GetConstReference() const { return stored; }
	typename Tr::ConstReference operator*() const { return stored; }
	typename Tr::ConstPointer operator->() const { return &stored; }
};

template <class T>
class Storage : public ConstStorage<T>//, IStorage<T>
{
public:
	Storage(const ObjectConstructParams &P) : ConstStorage<T>(P) { }

	typedef typename Type::Traits<T> Tr;

	typename Tr::Reference GetReference() 
	{ 
		StorageBase::SetDirty();
		return ConstStorage<T>::stored;
	}

	typename Tr::Reference operator*() /*const*/ { return GetReference(); }
	typename Tr::Pointer operator->() /*const*/ { return &GetReference(); }

	typename Tr::Reference GetCleanReference()
	{
		return ConstStorage<T>::stored;
	}
};

template <class T>
class Storage<const T> : ConstStorage<T>
{
	Storage() { }
	Storage(const ObjectConstructParams &P) : ConstStorage<T>(P) { }
	typedef typename ConstStorage<T>::Traits Tr;

	typename Tr::Reference GetReference() 
	{ 
		KAI_THROW_0(ConstError); 
	}
};

KAI_END

