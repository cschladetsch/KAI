#include <KAI/Core/Config/Base.h>
#include <KAI/Core/Object/Object.h>
#include <KAI/Core/Debug.h>
#include <iostream>

using namespace std;

KAI_BEGIN

StringStream& operator<<(StringStream &s, const Structure & st)
{
	s;
}

namespace debug
{
	StringStream& Trace::operator<<(Object const&Q)
	{
		return *this << Q.ToString();
	}

	const char *TypeToString(Trace::Type t)
	{
		switch (t)
		{
			#define CASE(V) case Trace::V: return #V;
		case Trace::Information: return "Info";
			CASE(Warn);
			CASE(Error);
			CASE(Fatal);
		}

		return "??";
	}

	Trace::~Trace()
	{
#if defined(KAI_TRACE_VERBOSE) || defined(KAI_TRACE_FILE_LOCATION)
		auto val = ToString();
		cout << file_location.ToString(true).c_str() << TypeToString(type) << ": " << val.c_str() << endl;
#else
		auto lead = TypeToString(type);
		if (lead != 0)
			cout << lead << ": ";
		cout << ToString().c_str() << endl;
#endif
	}
}

KAI_END

