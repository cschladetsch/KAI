#include <KAI/Core/Config/Base.h>
#include <KAI/Core/Object/Object.h>
#include <KAI/Core/Debug.h>
#include <iostream>

KAI_BEGIN

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
			case Trace::Information: return 0;
			CASE(Warn);
			CASE(Error);
			CASE(Fatal);
		}

		return "Toshing";
	}

	Trace::~Trace()
	{
#if defined(KAI_TRACE_VERBOSE) || defined(KAI_TRACE_FILE_LOCATION)
		std::cout << file_location.ToString(true).c_str() << ": " << TypeToString(type) << ": " << ToString().c_str() << std::endl;
#else
		auto lead = TypeToString(type);
		if (lead != 0)
			std::cerr << lead << ": ";
		std::cerr << ToString().c_str() << std::endl;
#endif
	}
}

KAI_END

