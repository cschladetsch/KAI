#pragma once

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

KAI_END
