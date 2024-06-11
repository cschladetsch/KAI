#pragma once

#include "KAI/Network/Config.h"
#include "KAI/Network/NetHandle.h"

KAI_NET_BEGIN

struct NetObject : Object {
    NetHandle _netHandle;
    Domain *_domain;

   public:
    NetHandle GetNetHandle() const;
};

KAI_NET_END
