#pragma once

#include "KAI/Net/Net.h"

KAI_NET_BEGIN

struct Node
{
	typedef std::unordered_map<NetHandle, std::shared_ptr<ProxyCommon>, HashNetHandle> Proxies;
	typedef std::unordered_map<NetHandle, std::shared_ptr<AgentCommon>, HashNetHandle> Agents;

	Agents agemts;
	Proxies proxies;

	Node();

	void Listen(int listen);
	void Connect(IpAddress const &, int port);
};

KAI_NET_END
