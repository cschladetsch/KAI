#pragma once

#include "KAI/Network/NetHandle.h"
#include "KAI/Network/Representative.h"
#include "KAI/Network/FwdDeclarations.h"
#include "KAI/Network/NetObject.h"

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
