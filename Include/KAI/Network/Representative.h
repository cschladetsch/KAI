#pragma once

#include "KAI/Network/Config.h"
#include "KAI/Network/FwdDeclarations.h"
#include "KAI/Network/NetHandle.h"

KAI_NET_BEGIN

NetHandle GetNetHandle(Object const &t, Node const &);

// common to either proxy or agent
class Representative : Reflected
{
public:
	Representative(Node &node, NetHandle handle)
		: _node(node), _netHandle(handle)
	{
	}

protected:
	virtual void Receive(NetHandle, BinaryPacket &packet) = 0;
	virtual void Receive(NetHandle, StringStream &packet) = 0;

	void Send(NetHandle, const char *);
	void Send(NetHandle, BinaryPacket const &);

	Node &_node;
	NetHandle _netHandle;
};

KAI_NET_END
