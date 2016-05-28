#pragma once

#include <Tau/Config.h>

TAU_BEGIN

struct Conversion
{
	/// @param from the idl
	/// @param to the C++
	Conversion(const char *from, const char *to);
};

void Process(const char *idl, const char *agents, const char *proxies_);

TAU_END


