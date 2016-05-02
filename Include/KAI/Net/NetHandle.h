#pragma once

#include "KAI/Net/Node.h"

KAI_NET_BEGIN

struct NetHandle
{
	MacAddress MacAddress;
	Handle Handle;

	friend bool operator==(NetHandle const &A, NetHandle const & B)
	{
		return A.MacAddress == B.MacAddress && A.Handle == B.Handle;
	}
};

struct HashNetHandle
{
	size_t operator()(NetHandle const &) const
	{
		return 3;
	}
};

KAI_NET_END

