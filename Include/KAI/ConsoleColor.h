#pragma once

KAI_BEGIN

struct Color
{
	enum Type
	{
		Prompt,
		Input,
		Trace,
		Error,
		Warning,
		StackEntry,
		StackIndex,
		Last
	};

	Color();
	~Color();

	static void SetColor(Type c);
};

inline std::ostream& operator<<(std::ostream& out, Color::Type c)
{
	Color::SetColor(c);
	return out;
}

KAI_END

