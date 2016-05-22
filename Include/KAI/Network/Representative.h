#pragma once

#include "KAI/Network/Config.h"
#include "KAI/Network/FwdDeclarations.h"
#include "KAI/Network/NetHandle.h"

KAI_NET_BEGIN

NetHandle GetNetHandle(Object const &t, Node const &);

// common to either proxy or agent
struct Representative : Reflected
{
	NetHandle Handle;

	Representative(Node &node, Object const &ob)
		: _node(node), _netHandle(GetNetHandle(ob, node)), _object(ob)
	{
	}

	virtual void Receive(NetHandle, BinaryPacket &packet) = 0;
	virtual void Receive(NetHandle, StringStream &packet) = 0;

protected:
	Node &_node;
	NetHandle _netHandle;
	Object _object;
};

KAI_NET_END
