#pragma once

#include "KAI/Network/Config.h"
#include "KAI/Network/Future.h"
#include "KAI/Network/Node.h"
#include "KAI/Network/ProxyBase.h"

KAI_NET_BEGIN

template <class T>
struct Proxy : ProxyBase {
    Proxy(Node &node, NetHandle remote) : ProxyBase(node, remote) {}

   private:
};

KAI_NET_END
