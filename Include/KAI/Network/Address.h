#pragma once

#include <string>
#include "KAI/Network/Config.h"

KAI_NET_BEGIN

struct MacAddress
{
	const std::string &Text() const;

	friend bool operator==(MacAddress const &A, MacAddress const &B)
	{
		KAI_UNUSED_2(A, B);
		return false;
	}

	size_t Hash() const
	{
		//MUST return boost::hash<string>(text);
		return 3; 
	}

private:
	std::string _text;
};

struct IpAddress
{
	const std::string &Text() const;

	friend bool operator==(IpAddress const &A, IpAddress const &B)
	{
		KAI_UNUSED_2(A, B);
		return false;
	}
};

KAI_NET_END

