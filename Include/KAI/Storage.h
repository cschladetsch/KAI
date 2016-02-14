

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_STORAGE_H
#	define KAI_STORAGE_H

KAI_BEGIN

template <class T>
struct ConstStorage : StorageBase//, IConstStorage<T>
{
	typedef typename Type::Traits<T> Traits;
	typedef typename Traits::Store Stored;

protected:
	Stored stored;

public:
	ConstStorage(const ObjectConstructParams &P) : StorageBase(P) { SetClean(); }

	typename Traits::ConstReference GetConstReference() const { return stored; }
	typename Traits::ConstReference operator*() const { return stored; }
	typename Traits::ConstPointer operator->() const { return &stored; }
};

template <class T>
struct ConstStorage<Container<T> > : StorageBase//, IConstStorage<T>
{
	typedef typename Type::Traits<T> Traits;
	typedef typename Traits::Store Stored;

protected:
	Stored stored;

public:
	ConstStorage(const ObjectConstructParams &P) : StorageBase(P), stored(5/*P.registry->GetAllocator().GetHeap()*/) { SetClean(); }

	typename Traits::ConstReference GetConstReference() const { return stored; }
	typename Traits::ConstReference operator*() const { return stored; }
	typename Traits::ConstPointer operator->() const { return &stored; }
};

template <class T>
struct Storage : ConstStorage<T>//, IStorage<T>
{
public:
	Storage(const ObjectConstructParams &P) : ConstStorage<T>(P) { }

	typedef typename ConstStorage<T>::Traits Traits;

	typename Traits::Reference GetReference() 
	{ 
		StorageBase::SetDirty();
		return ConstStorage<T>::stored;
	}

	typename Traits::Reference operator*() /*const*/ { return GetReference(); }
	typename Traits::Pointer operator->() /*const*/ { return &GetReference(); }

	typename Traits::Reference GetCleanReference()
	{
		return ConstStorage<T>::stored;
	}
};

template <class T>
struct Storage<const T> : ConstStorage<T>
{
	Storage() { }
	Storage(const ObjectConstructParams &P) : ConstStorage<T>(P) { }
	typedef typename ConstStorage<T>::Traits Traits;

	typename Traits::Reference GetReference() 
	{ 
		KAI_THROW_0(ConstError); 
	}
};

KAI_END

#endif // KAI_STORAGE_H

//EOF
