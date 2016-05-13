#include "KAI/KAI.h"

KAI_BEGIN

String Label::ToString() const
{
	return value;
}

void Label::FromString2(String text)
{
	FromString(text);
}

void Label::FromString(const Value &S)
{
	value = "";

	// TODO: validation

	if (S.empty())
		return;

	const String::Char *str = S.c_str();
	value = str;
}

BinaryStream &operator<<(BinaryStream &S, Label const &L)
{
	return S << L.GetValue();
}

BinaryPacket &operator>>(BinaryPacket &S, Label &L)
{
	String value;
	S >> value;
	L.FromString(value);
	return S;
}

void Label::Register(Registry &R)
{
	ClassBuilder<Label>(R, Label("Label"))
		//.Methods
			//(&Label::ToString, "ToString")
		;
}

KAI_END

