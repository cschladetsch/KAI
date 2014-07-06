
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
	L.FromString(value);
	return S;
}

void Label::Register(Registry &R)
{
	ClassBuilder<Label>(R, "Label")
		.Methods
			("ToString", &Label::ToString)
			("FromString", &Label::FromString2)
		.Properties
		;
}

//EOF
