#include "KAI/Language/Common/Process.h"
#include <stdarg.h>

using namespace std;

KAI_BEGIN

bool Process::Fail(const string &err) const
{
	Failed = true;
	Error = move(err);

	return false;
}

bool Process::Fail(const char *fmt, ...) const
{
	va_list ap;
	va_start(ap, fmt);
	char buffer[BUFSIZ];
	#ifdef WIN32
	vsprintf_s(buffer, fmt, ap);
	#else
	vsprintf(buffer, fmt, ap);
	#endif

	return Fail(move(string(buffer)));
}

KAI_END

