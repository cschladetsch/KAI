#include  "KAI/Net/Net.h"
#include  "KAI/Net/Representative.h"

KAI_NET_BEGIN

struct ProxyCommon : Representative
{
	ProxyCommon(NetHandle handle) : Representative(handle) { } 
};

KAI_NET_END
