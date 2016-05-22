#pragma once

#include "KAI/Network/Representative.h"

KAI_NET_BEGIN

struct ProxyCommon : Representative
{
	ProxyCommon(Node &node, Object const &obj)
		: Representative(node, obj) { }
};

KAI_NET_END
