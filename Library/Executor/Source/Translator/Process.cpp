#include "KAI/Language/Common/Process.h"

#include <stdarg.h>

KAI_BEGIN

bool Process::Fail(const std::string &err) const
{
	Failed = true;
	Error = err;

	return false;
}

bool Process::Fail(const char *fmt, ...) const
{
	va_list ap;
	va_start(ap, fmt);
	char buffer[1000];
	#ifdef WIN32
	vsprintf_s(buffer, fmt, ap);
	#else
	vsprintf(buffer, fmt, ap);
	#endif

	return Fail(std::string(buffer));
}

KAI_END

