#include <map>

#include "KAI/Console/Color.h"
#include "KAI/Core/Debug.h"
#include "KAI/Core/StringStream.h"


KAI_BEGIN

static Color _color;

struct Color::Impl
{
	std::map<int, std::string> colors;
};

static std::string AddEscape(const char *text)
{
	std::string s;
	s += (char)27;
	s += text;
	return s;
}

Color::Color()
	: _impl(new Impl())
{
	_impl->colors[Normal] = AddEscape("[0m");
	_impl->colors[Error] = AddEscape("[0;31m");
	_impl->colors[Warning] = AddEscape("[0;33m");
	_impl->colors[Trace] = AddEscape("[0;34m");
	_impl->colors[StackNumber] = AddEscape("[1;30m");
	_impl->colors[Prompt] = AddEscape("[1;37m");
	_impl->colors[LanguageName] = AddEscape("[0;34m");
	_impl->colors[Input] = AddEscape("[1;33m");
}

std::string Color::GetColor(EType type) const
{
	if (_impl->colors.find(type) == _impl->colors.end())
	{
		return _impl->colors[Input];
	}

	return _impl->colors[type];
}

std::ostream& operator<<(std::ostream &S, Color::EType C)
{
	return S;// << _color.GetColor(C);
}

KAI_END
