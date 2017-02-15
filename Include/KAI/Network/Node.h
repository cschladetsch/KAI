#pragma once

#include "KAI/Network/Network.h"

namespace RakNet
{
	class RakPeerInterface;
}

KAI_NET_BEGIN

// A peer on the network. Nominally, there are no servers or clients. Just
// a collection of nodes that connect and communicate with each other.
struct Node
{
private:
	RakNet::RakPeerInterface *_peer;

public:
	static int constexpr DefaultPort = 14589;

	Node();

	void Listen(int port);
	void Listen(IpAddress const &address, int port);

	void Connect(IpAddress const &, int port);

	template <class T = void>
	Future<T> Send(NetHandle, const Object&);

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

