#pragma once

#include <KAI/Core/Config/Base.h>
#include <sstream>
#include <memory>
#include <string>

KAI_BEGIN

class ConsoleColor
{
	struct Impl;
	std::unique_ptr<Impl> _impl;

public:
	enum EType
	{
		Normal,
		Error,
		Warning,
		Trace,
		StackNumber,
		Prompt,
		LanguageName,
		Pathname,
		Input,

		Last,
	};

	enum EConsoleColor
	{
		Red, Green, Blue,
	};

	std::string GetConsoleColor(EType type) const;
};

std::ostream& operator<<(std::ostream &S, ConsoleColor C);

KAI_END
