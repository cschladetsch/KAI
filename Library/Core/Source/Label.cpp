
#include "KAI/Core/BuiltinTypes/All.h"

KAI_BEGIN

String Label::ToString() const
{
	if (!Quoted)
		return value;
	return String("'" + value);
}

void Label::FromString2(String text)
{
	FromString(text);
}

void Label::FromString(const Value &S)
{
	value = "";
	if (S.empty())
		return;

	const String::Char *str = S.c_str();
	if (str[0] == '\'')
	{
		Quoted = true;
		str++;
	}
	value = str;
}
StringStream &operator<<(StringStream &S, const Label &L)
{
	if (L.Quoted)
		S << "'";
	return S << L.GetValue();
}

StringStream &operator>>(StringStream &S, Label &L)
{
	L.Quoted = S.Peek() == '\'';
	if (L.Quoted)
	{
		char ch;
		S.Extract(ch);
	}

	String val;
	S >> val;
	L.FromString(val);
	return S;
}


BinaryStream &operator<<(BinaryStream &S, Label const &L)
{
	return S << L.Quoted << L.GetValue();
}

BinaryStream &operator>>(BinaryStream &S, Label &L)
{
	String value;
	bool quoted;
	S >> quoted >> value;
	L.FromString(value);
	L.Quoted = quoted;
	return S;
}

void Label::Register(Registry &R)
{
	ClassBuilder<Label>(R, Label("Label"))
		.Methods
			("ToString", &Label::ToString, "ToString")
			("FromString", &Label::FromString2, "FromString")
		;
}

KAI_END

