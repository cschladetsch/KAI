#pragma once

#include "boost/date_time/posix_time/posix_time_types.hpp"
#include "boost/date_time/time_zone_base.hpp"
#include "boost/date_time/local_time/local_time.hpp"
#include "KAI/Network/Config.h"

KAI_NET_BEGIN

struct DateTime
{
	boost::local_time::local_date_time Now;
};

KAI_NET_END


