#pragma once

#include "KAI/Net/Base.h"

KAI_NET_BEGIN

struct MacAddress
{
	const std::string &Text() const;
};

struct IpAddress
{
	const std::string &Text() const;
};

KAI_NET_END
