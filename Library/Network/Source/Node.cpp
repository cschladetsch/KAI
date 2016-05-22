#include "KAI/Network/Node.h"

KAI_NET_BEGIN

Node::Node()
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

Future Node::Send(NetHandle, Object)
{
	KAI_NOT_IMPLEMENTED();
	return Future();
}

Future Node::Receive(NetHandle, Object)
{
	KAI_NOT_IMPLEMENTED();
	return Future();
}

KAI_NET_END
