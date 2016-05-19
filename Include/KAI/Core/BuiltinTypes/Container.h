#pragma once

#include <KAI/Core/Config/Base.h>
#include <KAI/Core/Reflected.h>
#include <KAI/Core/ObjectConstructParams.h>

KAI_BEGIN

template <class T>
struct Container : Reflected//<T>
{
	bool Attach(Object const &Q)
	{
		if (!Self)
		{
			// it is wrong to try to attach to a null container
			KAI_THROW_0(NullObject);
			//return false;
		}
		if (!Q)
		{
			// it is wrong to try to attach a null object to a container
			KAI_THROW_0(NullObject);
			//return false;
		}

		if (Q.IsMarked())
			return false;
		Q.AddedToContainer(*Self);
		return true;
	}
	void Detach(Object const &Q)
	{
		if (!Self || !Q)
			return;//KAI_THROW_0(NullObject);
		Q.RemovedFromContainer(*Self);
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
	ConstStorage(const ObjectConstructParams &P)
		: StorageBase(P)//, stored(P.registry->GetAllocator().GetHeap()*/)
	{ 
		SetClean(); 
	}

	typename Tr::ConstReference GetConstReference() const { return stored; }
	typename Tr::ConstReference operator*() const { return stored; }
	typename Tr::ConstPointer operator->() const { return &stored; }
};

KAI_END

