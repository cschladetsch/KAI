#pragma once

#include <KAI/Core/Config/Base.h>
#include <string>

KAI_BEGIN

struct Process
{
	bool Failed;
	std::string Error;

	Process() : Failed(false) { }

	static int trace;

protected:
	bool Fail(const char *fmt, ...);
	bool Fail(const std::string &);
};

KAI_END
