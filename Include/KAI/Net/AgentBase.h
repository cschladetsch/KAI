#pragma once

#include "KAI/Net/Base.h"
#include "KAI/Net/NetHandle.h"

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
	ResponseType Result;

	DateTime Sent;
	DateTime Received;

	NetHandle Sender;
	NetHandle Recipient;

	Object Request;
	Object Result;
};

template <class T>
struct Future
{
	typedef T Message;
	
	bool Arrived;
	Message Message;
};

template <class T>
struct AgentBase : AgentCommon
{
	typedef T  Servant;

	Future<Message> Request(ProxyBase<T> proxy, Object request);

private:
	std::shared_ptr<Servat> _servant;
};

KAI_NET_END
