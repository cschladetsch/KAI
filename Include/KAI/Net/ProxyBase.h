#pragma once

#include "Kai/Net/Base.h"
#include "Kai/Net/ProxyCommon.h"

KAI_NET_BEGIN

template <class T>
struct ProxyBase : ProxyCommon
{
	typedef T Server;

	Future Request(ProxyBase<T> proxy, Object request);

private:
};

KAI_NET_END
