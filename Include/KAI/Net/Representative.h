#pragma once

#include "KAI/Net/Base.h"

KAI_NET_BEGIN

// common to either proxy or agent
struct Representative : Reflected
{
	NetHandle Handle;

	Representative() { }
	Representative(const NetHandle nh);

	virtual void Receive(BinaryPacket &packet) = 0;
	virtual void Receive(StringStream &packet) = 0;
};

KAI_NET_END
