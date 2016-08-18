#pragma once

#include <KAI/Network/Representative.h>
#include <KAI/Network/Future.h>

KAI_NET_BEGIN

struct ProxyBase : Representative
{
protected:
	typedef StringStream StreamType;

	ProxyBase(Node &node, NetHandle handle)
		: Representative(node, handle) { }

	template <class Ty>
	Future<Ty> Exec(const char *name, StreamType &args)
	{
		return Future<Ty>();
	}

	template <class Ty>
	Future<Ty> Fetch(const char *name)
	{
		return Future<Ty>();
	}
};

KAI_NET_END

