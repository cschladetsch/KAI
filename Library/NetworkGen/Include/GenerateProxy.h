#pragma once

#include <Tau/Config.h>
#include <Tau/TauParser.h>

TAU_BEGIN

struct WriteProxy
{
	void Walk(TauParser &&);
};

TAU_END

