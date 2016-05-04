#pragma once

#include "KAI/Net/Base.h"
#include "KAI/Net/NetHandle.h"

KAI_NET_BEGIN

template <class T>
struct AgentBase : AgentCommon
{
	typedef T  Servant;

	AgentBase(Node &node, std::shared_ptr<Servant> server)
		: AgentCommon(node), _server(server)
	{
	}

	Future Respond(NetHandle handle, TypeNumber ty, Object request);

private:
	std::shared_ptr<Servant> _servant;
};

KAI_NET_END
