#pragma once

#include "KAI/Network/Representative.h"

KAI_NET_BEGIN

struct ProxyBase : Representative
{
	ProxyBase(Node &node, NetHandle handle);
};

KAI_NET_END
