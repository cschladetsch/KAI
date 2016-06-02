#include "KAI/Network/Node.h"
#include <raknet/RakPeerInterface.h>

KAI_NET_BEGIN

using namespace RakNet;
using namespace std;

struct Node::Impl
{
	int constexpr NumSockets = 100;
	int constexpr Port = 6666;

	shared_ptr<RakPeerInterface> _peer;
	SocketDescriptor _sockets[NumSockets];

	Impl()
		: _peer(make_shared<RakPeerInterface>())
	{
		for (auto &sock : _sockets)
		{
			sock.port = Port;
			sock.blockingSocket = false;
		}

		_peer->Startup(200, _sockets, NumSockets);
	}

	void Something()
	{
	}
};

Node::Node()
	: _impl(std::make_shared<Impl>())
{
}

void Node::Listen(int port)
{
	KAI_UNUSED_1(port);
	KAI_NOT_IMPLEMENTED();
}

void Node::Connect(IpAddress const &, int port)
{
	KAI_UNUSED_1(port);
	KAI_NOT_IMPLEMENTED();
}

//Future Node::Send(NetHandle, Object)
//{
//	KAI_NOT_IMPLEMENTED();
//	return Future();
//}
//
//Future Node::Receive(NetHandle, Object)
//{
//	KAI_NOT_IMPLEMENTED();
//	return Future();
//}

KAI_NET_END
