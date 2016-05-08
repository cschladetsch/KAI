#pragma once

#include "KAI/Net/Base.h"
#include "KAI/Net/NetHandle.h"
#include "KAI/Net/Representative.h"

KAI_NET_BEGIN

struct ProxyCommon : Representative
{
	ProxyCommon(Node &node, Object const &obj)
		: Representative(node, obj) { }
};

KAI_NET_END
