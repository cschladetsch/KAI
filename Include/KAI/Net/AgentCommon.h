#pragma once

#include "KAI/Net/Base.h"
#include "KAI/Net/NetHandle.h"
#include "KAI/Net/Representative.h"

KAI_NET_BEGIN

struct AgentCommon : Representative
{
	AgentCommon(Node &node, Object servant)
		: Representative(node, servant) 
	{ 
	}
};

KAI_NET_END
