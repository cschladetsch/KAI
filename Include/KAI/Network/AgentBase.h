#pragma once

#include "KAI/Network/Config.h"
#include "KAI/Network/NetHandle.h"
#include "Network.h"

KAI_NET_BEGIN

template <class T>
struct AgentBase : AgentCommon
{
	typedef T  Servant;

	AgentBase(Node &node, Pointer<Servant> server)
		: AgentCommon(node, server)
	{
	}

	Future Respond(NetHandle handle, TypeNumber ty, Object request);

private:
	Pointer<Servant> _servant;
};

KAI_NET_END
