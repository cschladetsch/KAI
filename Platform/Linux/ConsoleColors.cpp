//
// Created by Christian on 11/02/2017.
//

#include "KAI/Console/Color.h"
#include "./Include/ColorEscapeSequences.h"
#include <map>
#include <string>

KAI_BEGIN

static Color _color;

struct Color::Impl
{
	std::map<int, std::string> colors;
};

static std::string AddEscape(const char *text)
{
	std::string s;
	s += '[';
	s += (char)27;
	s += text;
	return s;
}

Color::Color()
	: _impl(new Impl())
{
	_impl->colors[Normal] = AddEscape("[1;31m");
	_impl->colors[Error] = AddEscape("[1;31m");
	_impl->colors[Warning] = AddEscape("[1;31m");
	_impl->colors[Trace] = AddEscape("[1;31m");
	_impl->colors[StackNumber] = AddEscape("[1;31m");
	_impl->colors[Prompt] = AddEscape("[1;31m");
	_impl->colors[LanguageName] = AddEscape("[1;31m");
	_impl->colors[Input] = AddEscape("[1;31m");
}

std::string Color::GetColor(EType type) const
{
	if (_impl->colors.find(type) == _impl->colors.end())
		return std::string();

	return std::move(_impl->colors[type]);
}

std::ostream& operator<<(std::ostream &S, Color::EType C)
{
	std::string D = _color.GetColor(C);
	return S << _color.GetColor(C);
}

KAI_END
