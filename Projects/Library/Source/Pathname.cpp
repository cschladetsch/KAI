#include "KAI/KAIClassBuilder.h"

#include <ctype.h>

KAI_BEGIN

const String::Char Pathname::Literals::Parent = '^';
const String::Char Pathname::Literals::This = '.';
const String::Char Pathname::Literals::Seperator = '/';
const String::Char Pathname::Literals::Quote = '\'';
const String::Char Pathname::Literals::All[]= { Parent, This , Seperator, Quote, 0 };
const String::Char Pathname::Literals::AllButQuote[]= { Parent, This , Seperator, 0 };

Pathname::Pathname(const String &text)
{
	FromString(text);
}

void Pathname::FromString2(String text)
{
	FromString(text);
}

void Pathname::FromString(const String &text)
{
	elements.clear();
	if (text.empty())
		return;

	const String::Char *S = text.c_str();

	Absolute = *S == Literals::Seperator;

	if (Absolute)
		++S;

	StringStream name;
	for (; *S; ++S)
	{
		switch (*S)
		{
		case Literals::Parent:
			AddElement(name, Element::Parent);
			break;

		case Literals::Seperator:
			if (S[1] != 0)
				AddElement(name, Element::Seperator);
			break;

		case Literals::This:
			AddElement(name, Element::This);
			break;

		default:
			if (!isalnum(*S) && *S != '_')
			{
				elements.clear();
				KAI_THROW_1(InvalidPathname, text);
			}
			name.Append(*S);
			break;
		}
	}

	name << Ends;
	String s = name.ToString();
	if (!s.Empty())
		elements.push_back(Element(s));

	if (elements.empty())
		return;
	
	if (elements.front().type == Element::Seperator)
	{
		Absolute = true;
		elements.pop_front();
	}

	if (elements.back().type == Element::Seperator)
		elements.pop_back();

	if (!Validate())
	{
		elements.clear();
		KAI_THROW_1(InvalidPathname, text);
	}
}

void Pathname::AddElement(StringStream &name, Element::Type type)
{
	if (!name.Empty())
	{
		name << Ends;
		elements.push_back(Element(name.ToString()));
	}

	elements.push_back(type);
	name.Clear();
}

String Pathname::ToString() const
{
	StringStream str;
	if (Absolute)
		str.Append(Literals::Seperator);

	for (auto element : elements)
	{
		switch (element.type)
		{
		case Element::Seperator: str.Append(Literals::Seperator);
			break;
		
		case Element::Parent: str.Append(Literals::Parent); 
			break;

		case Element::This: str.Append(Literals::This); 
			break;

		case Element::Name: str << element.name.ToString(); 
			break;

		case Element::None:
			break;
		}
	}

	str << Ends;
	return str.ToString();
}

bool Pathname::Empty() const
{
	return elements.empty();
}

bool Pathname::Validate() const
{
	if (elements.empty())
		return true;
	// TODO
	return true;
}

StringStream &operator<<(StringStream &S, Pathname const &P)
{
	return S << P.ToString();
}

bool operator<(const Pathname &A, const Pathname &B)
{
	return A.elements < B.elements;
}

bool operator==(const Pathname &A, const Pathname &B)
{
	return A.elements == B.elements;
}

BinaryPacket &operator>>(BinaryPacket &, Pathname &)
{
	KAI_NOT_IMPLEMENTED();
}

BinaryStream &operator<<(BinaryStream &, const Pathname &)
{
	KAI_NOT_IMPLEMENTED();
}

void Pathname::Register(Registry &R)
{
	ClassBuilder<Pathname>(R, "Pathname")
		.Methods
			("Empty", &Pathname::Empty)
			("ToString", &Pathname::ToString)
			("FromString", &Pathname::FromString2)
		.Properties
			("absolute", &Pathname::Absolute)
		;
}

KAI_END

//EOF
