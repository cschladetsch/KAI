#pragma once

#include "KAI/Network/Config.h"
#include "KAI/Network/NetHandle.h"
#include "KAI/Network/DateTime.h"
#include "KAI/Network/ResponseType.h"

KAI_NET_BEGIN

template <class T = void>
struct Future
{
	// Id that is unique to the creator of the Future
	int Id;
	
	// the response type
	ResponseType Response;
	
	// if true, this future has been completed one way or another
	bool Complete;
	
	// When the future was completd
	DateTime When;
	
	// the pointer to the result of the future. used by a shared NetworkDomain
	Pointer<T> Result;
};

KAI_NET_END
