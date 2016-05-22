#pragma once

#include "KAI/Network/ProxyCommon.h"
#include "KAI/Network/Config.h"
#include "KAI/Network/Future.h"
#include "KAI/Network/Node.h"

KAI_NET_BEGIN

template <class T>
struct ProxyBase : ProxyCommon
{
	typedef T Server;

	ProxyBase(Node &node, const Object &obj) : ProxyCommon(node, obj)
	{
	}

	Future Request(ProxyBase<T> proxy, Object request);

private:
};

KAI_NET_END
