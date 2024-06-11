#pragma once

#include "KAI/Core/Object/Handle.h"
#include "KAI/Network/Address.h"

KAI_NET_BEGIN

struct NetHandle {
    MacAddress LocalMacAddress;
    Handle LocalHandle;
    int TypeNumber;

    friend bool operator==(NetHandle const &A, NetHandle const &B) {
        return A.LocalMacAddress == B.LocalMacAddress &&
               A.LocalHandle == B.LocalHandle && A.TypeNumber == B.TypeNumber;
    }
};

struct HashNetHandle {
    size_t operator()(NetHandle const &h) const {
        return h.LocalMacAddress.Hash() ^ h.LocalHandle.GetValue() ^
               h.TypeNumber;
    }
};

KAI_NET_END
