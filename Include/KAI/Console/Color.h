#pragma once

#include <KAI/Core/Config/Base.h>
//#include <memory>
#include <sstream>

KAI_BEGIN

// Generic color class for all consoles on all platforms
class Color
{
public:
	enum EType
	{
		Normal,
		Error,
		Warning,
		Trace,
		StackNumber,
		StackPutput,
		Prompt,
		LanguageName,
		Pathname,
		Input,

		Last,
	};

	enum EColor
	{
		Red, Green, Blue,       // .. etc
	};

	Color();
};

std::stringstream& operator<<(std::stringstream &S, Color::EType C);

KAI_END


