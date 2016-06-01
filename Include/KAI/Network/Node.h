#pragma once

#include "KAI/Network/Network.h"

KAI_NET_BEGIN

struct Node
{
	Node();

	void Listen(int listen);

	void Connect(IpAddress const &, int port);

	template <class T = void>
	Future<T> Send(NetHandle, Object);

	template <class T = void>
	Future<T> Receive(NetHandle, Object);

private:
	std::shared_ptr<Registry> _reg;

private:
	typedef std::unordered_map<NetHandle, std::shared_ptr<ProxyBase>, HashNetHandle> Proxies;
	typedef std::unordered_map<NetHandle, std::shared_ptr<AgentBase>, HashNetHandle> Agents;

	Agents _agents;
	Proxies _proxies;
};

KAI_NET_END

