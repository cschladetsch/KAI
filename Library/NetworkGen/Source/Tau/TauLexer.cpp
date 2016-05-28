#include "Tau/TauLexer.h"

#include <strstream>
#include <stdarg.h>
#include <algorithm>

using namespace std;

TAU_BEGIN

void TauLexer::AddKeyWords()
{
	keyWords["namespace"] = Enum::Namespace;
	keyWords["class"] = Enum::Class;
	keyWords["sync"] = Enum::Sync;
	keyWords["async"] = Enum::Async;
	keyWords["Proxy"] = Enum::Proxy;
}

bool TauLexer::NextToken()
{
	char current = Current();
	if (current == 0)
		return false;

	if (isalpha(current))
		return LexAlpha();

	if (isdigit(current))
		return Fail("Number not expected");

	switch (current)
	{
	case ';': return Add(Enum::Semi);
	case '{': return Add(Enum::OpenBrace);
	case '}': return Add(Enum::CloseBrace);
	case '(': return Add(Enum::OpenParan);
	case ')': return Add(Enum::CloseParan);
	case ',': return Add(Enum::Comma);
	case ' ': return Add(Enum::Whitespace, Gather(IsSpaceChar));
//	case '@': return Add(Enum::Lookup);
//	case '*': return Add(Enum::Mul);
//	case '[': return Add(Enum::OpenSquareBracket);
//	case ']': return Add(Enum::CloseSquareBracket);
//	case '=': return AddIfNext('=', Enum::Equiv, Enum::Assign);
//	case '!': return AddIfNext('=', Enum::NotEquiv, Enum::Not);
//	case '&': return AddIfNext('&', Enum::And, Enum::BitAnd);
//	case '|': return AddIfNext('|', Enum::Or, Enum::BitOr);
//	case '<': return AddIfNext('=', Enum::LessEquiv, Enum::Less);
//	case '>': return AddIfNext('=', Enum::GreaterEquiv, Enum::Greater);
	case '"': return LexString();
	case '\'': return LexAlpha();
	case '\t': return Add(Enum::Tab);
	case '\n': return Add(Enum::NewLine);
//	case '-':
//		if (Peek() == '-')
//			return AddTwoCharOp(Enum::Decrement);
//		if (Peek() == '=')
//			return AddTwoCharOp(Enum::MinusAssign);
//		return Add(Enum::Minus);
//
//	case '.':
//		if (Peek() == '.')
//		{
//			Next();
//			if (Peek() == '.')
//			{
//				Next();
//				return Add(Enum::Replace, 3);
//			}
//			return Fail("Two dots doesn't work");
//		}
//		return Add(Enum::Dot);
//
//	case '+':
//		if (Peek() == '+')
//			return AddTwoCharOp(Enum::Increment);
//		if (Peek() == '=')
//			return AddTwoCharOp(Enum::PlusAssign);
//		return Add(Enum::Plus);

	case '/':
		if (Peek() == '/')
		{
			Next();
			int start = offset;
			while (Next() != '\n')
				;
			return Add(Enum::Comment, offset - start);
		}
		Fail("Expected comment start");
	}

	LexError("Unrecognised %c");

	return false;
}

void TauLexer::Terminate()
{
	Add(Enum::None, 0);
}

TAU_END