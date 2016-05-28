#pragma once

#include <KAI/Core/Config/Base.h>
#include <string>

KAI_BEGIN

struct Process
{
	static int trace;

	mutable bool Failed = false;
	mutable std::string Error;

protected:
	bool Fail(const char *fmt, ...) const;
	bool Fail(const std::string &) const;
};

KAI_END

