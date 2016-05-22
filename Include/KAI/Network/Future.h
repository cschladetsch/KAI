#pragma once

#include "KAI/Network/Config.h"

#include "NetHandle.h"
#include "DateTime.h"
#include "ResponseType.h"

KAI_NET_BEGIN

template <class T = void>
struct Future
{
	ResponseType Response;
	bool Complete;
	DateTime When;
	Pointer<T> Result;
};

KAI_NET_END
