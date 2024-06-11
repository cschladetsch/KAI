#pragma once

#include "KAI/Network/AgentBase.h"
#include "KAI/Network/NetHandle.h"
#include "NetPointer.h"

KAI_NET_BEGIN

template <class T>
struct Agent : AgentBase {
    typedef Pointer<T> Servant;

    Agent(Node &node, NetPointer<Servant> server) : AgentBase(node, server) {
        _servant = server;
    }

    template <class R = void>
    Future<R> Respond(NetHandle handle, TypeNumber ty, Object request) {
        return Future<R>();
    }

   private:
    Pointer<Servant> _servant;
};

KAI_NET_END
