
String Label::ToString() const
{
	StringStream S;
	if (Quoted)
		S.Append('\'');
	S << ToStringUnquoted();
	return S.ToString();
}

String Label::ToStringUnquoted() const
{
	return value;
}

void Label::FromString2(String text)
{
	FromString(text);
}

void Label::FromString(const Value &S)
{
	Quoted = false;
	value = "";

	// TODO: validation

	if (S.empty())
		return;

	const String::Char *str = S.c_str();
	Quoted = str[0] == Pathname::Literals::Quote;
	if (Quoted)
		++str;
	value = str;
}

BinaryStream &operator<<(BinaryStream &S, Label const &L)
{
	return S << L.Quoted << L.GetValue();
}

BinaryPacket &operator>>(BinaryPacket &S, Label &L)
{
	String value;
	S >> L.Quoted >> value;
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
			("quoted", &Label::Quoted)
		;
}

//EOF
