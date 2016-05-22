#pragma once

#include <KAI/Core/Config/Base.h>
#include <string>

KAI_BEGIN

struct Process
{
	bool Failed = false;
	static int trace;

	std::string Error;

protected:
	bool Fail(const char *fmt, ...);
	bool Fail(const std::string &);
};

KAI_END

