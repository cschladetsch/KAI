#pragma once

#include "KAI/Net/Base.h"

#include "boost/date_time/posix_time/posix_time_types.hpp"
#include "boost/date_time/time_zone_base.hpp"
#include "boost/date_time/local_time/local_time.hpp"

KAI_NET_BEGIN

enum class ResponseType
{
	None,
	Pending,
	Failed,
	Delivered,
	BadRequest,
	UnkownAgent,
};

struct DateTime
{
	boost::local_time::local_date_time Now;

	void test()
	{
		//Now.time_of_day.utc_time;
	}
};

struct TimeSpan
{
};

struct Message
{
	ResponseType ResultType;

	DateTime Sent;
	DateTime Received;

	NetHandle Sender;
	NetHandle Recipient;

	Object Request;
	Object Result;
};

struct Future
{
	bool Arrived;
	Object Response;
};

KAI_NET_END