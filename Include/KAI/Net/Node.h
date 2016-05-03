#pragma once

#include "KAI/Net/Net.h"

#include <boost/asio.hpp>

KAI_NET_BEGIN

struct Node
{
	Node();

	void Listen(int listen);
	void Connect(IpAddress const &, int port);

	Future Send(NetHandle, Object);
	Future Receive(NetHandle, Object);

private:
	Registry _reg;
	boost::asio::io_service _io;

private:
	typedef std::unordered_map<NetHandle, std::shared_ptr<ProxyCommon>, HashNetHandle> Proxies;
	typedef std::unordered_map<NetHandle, std::shared_ptr<AgentCommon>, HashNetHandle> Agents;

	Agents agemts;
	Proxies proxies;

};

KAI_NET_END
