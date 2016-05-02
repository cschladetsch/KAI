#pragma once

KAI_NET_BEGIN

struct MacAddress
{
	const std::string &Text() const;

	friend bool operator==(MacAddress const &A, MacAddress const &B)
	{
		KAI_UNUSED_2(A, B);
		return false;
	}
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
