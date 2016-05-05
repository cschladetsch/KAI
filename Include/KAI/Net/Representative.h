#pragma once

#include "KAI/Net/Base.h"

KAI_NET_BEGIN

NetHandle GetNetHandle(Object const &t, Node const &);

// common to either proxy or agent
struct Representative : Reflected
{
	NetHandle Handle;

	Representative(Node &node, Object const &ob)
		: _node(node), _netHandle(GetNetHandle(ob, node))
	{
	}

	virtual void Receive(NetHandle, BinaryPacket &packet) = 0;
	virtual void Receive(NetHandle, StringStream &packet) = 0;

protected:
	Node &_node;
	NetHandle _netHandle;
};

KAI_NET_END
