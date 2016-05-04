#pragma once

#include "Kai/Net/Base.h"
#include "Kai/Net/NetHandle.h"
#include "Kai/Net/Representative.h"

KAI_NET_BEGIN

struct AgentCommon : Representative
{
	AgentCommon(Node &node, Object const &servant)
		: Representative(node, servant) 
	{ 
	}
};

KAI_NET_END
