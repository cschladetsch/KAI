#pragma once

#include "KAI/Network/NetHandle.h"
#include "KAI/Network/Representative.h"

KAI_NET_BEGIN

struct AgentCommon : Representative
{
	AgentCommon(Node &node, Object servant)
		: Representative(node, servant) 
	{ 
	}

protected:
	using Representative::_object;
};

KAI_NET_END
