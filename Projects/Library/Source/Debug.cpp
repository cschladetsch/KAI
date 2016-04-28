#include "KAI/KAI.h"
#include <iostream>

KAI_BEGIN

namespace debug
{
	const char *TypeToString(Trace::Type t)
	{
		switch (t)
		{
			#define CASE(V) case Trace::V: return #V;
			CASE(Information);
			CASE(Warn);
			CASE(Error);
			CASE(Fatal);
		}

		return "Toshing";
	}

	Trace::~Trace()
	{
		std::cerr << file_location.ToString(true).c_str() << ": " << TypeToString(type) << ": " << ToString().c_str() << std::endl;
	}
}

KAI_END
