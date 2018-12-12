#include <RakNet/RakPeer.h>

#include "KAI/Network/Node.h"

KAI_NET_BEGIN

using namespace RakNet;
using namespace std;

// struct Node::Impl
// {
// 	static const int constexpr NumSockets = 100;
// 	static const int constexpr Port = 6666;

// 	RakPeerInterface *_peer;
// 	SocketDescriptor _sockets[NumSockets];

// 	Impl()
// 		: _peer(make_unique<RakPeer>())
// 	{
// 		for (auto &sock : _sockets)
// 		{
// 			sock.port = Port;
// 			sock.blockingSocket = false;
// 		}

// 		_peer->Startup(200, _sockets, NumSockets);
// 	}
// };

Node::Node()
{
	_peer = RakPeerInterface::GetInstance();
	// _peer->Startup(8,&sd1,1);
}

void Node::Listen(int port)
{
	_peer->AllowConnectionResponseIPMigration(false);
}

// void Node::Connect(Node &other)
// {
// 	// _other = &other;
// }

// connect to another peer at given address and port
void Node::Connect(IpAddress const &ip, int port)
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
