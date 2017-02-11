#pragma once

#include <KAI/Core/Config/Base.h>
#include <sstream>
#include <memory>

KAI_BEGIN

// Generic color class for all consoles on all platforms
class Color
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

	enum EColor
	{
		Red, Green, Blue,
	};

	Color();

	std::string GetColor(EType type) const;
};

extern Color ConsoleColors;

std::ostream& operator<<(std::ostream &S, Color::EType C);

KAI_END


