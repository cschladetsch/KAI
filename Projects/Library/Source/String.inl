

String String::LowerCase() const
{
	String result((int)string.size(), ' ');
	std::transform(string.begin(), string.end(), result.begin(), ::tolower);
	return result;
}

String String::UpperCase() const
{
	String result((int)string.size(), ' ');
	std::transform(string.begin(), string.end(), result.begin(), ::toupper);
	return result;
}

String String::Capitalise() const
{
	if (string.empty())
		return String();

	String result = string;
	result[0] = (String::Char)::toupper(result[0]);
	return result;
}

bool String::Contains(String const &) const
{
	KAI_NOT_IMPLEMENTED();
}

bool String::StartsWith(String const &) const
{
	KAI_NOT_IMPLEMENTED();
}

void String::ReplaceFirst(String const &what, String const &with)
{
	KAI_UNUSED_2(what, with);
	KAI_NOT_IMPLEMENTED();
}

void String::ReplaceLast(String const &what, String const &with)
{
	KAI_UNUSED_2(what, with);
	KAI_NOT_IMPLEMENTED();
}

void String::RemoveAll(String const &what)
{
	//throw;
	boost::algorithm::erase_all(*this, what);
}

bool String::EndsWith(String const &) const
{
	KAI_NOT_IMPLEMENTED();
}

BinaryStream &operator<<(BinaryStream &S, const String &T)
{
	int length = T.Size();
	S << length;
	if (length > 0)
		S.Write(length, &*T.Begin());

	return S;
}

BinaryPacket &operator>>(BinaryPacket &S, String &T)
{
	int length = 0;
	S >> length;
	if (length == 0)
	{
		T = "";
		return S;
	}

	// TODO: allocate from String directly
	char *buffer = new char[length + 1];
	S.Read(length, buffer);
	buffer[length] = 0;
	T = buffer;
	delete[] buffer;
	return S;
}

//EOF
