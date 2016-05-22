#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>

#include "KAI/Network/Network.h"

KAI_NET_BEGIN

struct Node
{
	Node();

	void Listen(int listen);

	void Connect(IpAddress const &, int port);

	Future Send(NetHandle, Object);

	Future Receive(NetHandle, Object);

private:
	std::shared_ptr<Registry> _reg;
	std::shared_ptr<boost::asio::io_service> _io;

private:
	typedef std::unordered_map<NetHandle, std::shared_ptr<ProxyCommon>, HashNetHandle> Proxies;
	typedef std::unordered_map<NetHandle, std::shared_ptr<AgentCommon>, HashNetHandle> Agents;

	Agents _agents;
	Proxies _proxies;
};

KAI_NET_END

