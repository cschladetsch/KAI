#pragma once

#include "KAI/Net/Net.h"

KAI_NET_BEGIN

struct NetHandle
{
	MacAddress MacAddress;
	Handle Handle;
	int TypeNumber;

	friend bool operator==(NetHandle const &A, NetHandle const & B)
	{
		return A.MacAddress == B.MacAddress && A.Handle == B.Handle && A.TypeNumber == B.TypeNumber;
	}
};

struct HashNetHandle
{
	size_t operator()(NetHandle const &h) const
	{
		return h.MacAddress.Hash() ^ h.Handle.GetValue() ^ h.TypeNumber;
	}
};

KAI_NET_END

