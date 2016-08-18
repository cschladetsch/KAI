#pragma once

#include "KAI/Network/Representative.h"

KAI_NET_BEGIN

struct AgentBase : Representative
{
	AgentBase(Node &node, NetObject servant)
		: Representative(node, servant.GetNetHandle())
	{ 
	}

protected:
//	using Representative::_object;
};

KAI_NET_END
