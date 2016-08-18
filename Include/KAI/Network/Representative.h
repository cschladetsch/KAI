#pragma once

#include "KAI/Network/Config.h"
#include "KAI/Network/FwdDeclarations.h"
#include "KAI/Network/NetObject.h"
#include "KAI/Network/NetHandle.h"

KAI_NET_BEGIN

NetHandle GetNetHandle(Object const &t, Node const &);

// common to either proxy or agent
struct Representative : Reflected
{
protected:
	Representative(Node &node, NetHandle handle)
		: _node(node), _netHandle(handle)
	{
	}

protected:
	void Receive(NetHandle sender, BinaryStream &packet);
	void Receive(NetHandle sender, StringStream &packet);

	void Send(NetHandle recipient, const char *);
	void Send(NetHandle recipient, BinaryPacket const &);

private:
	Node &_node;
	NetHandle _netHandle;
};

KAI_NET_END
