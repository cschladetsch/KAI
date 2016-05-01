#pragma once

#include <iostream>

KAI_BEGIN

// common to all Tokens
template <class EnumType>
struct TokenBase
{
	typedef TokenBase<EnumType> Self;
	typedef typename EnumType::Enum Enum;

	Enum type;
	Slice slice;
	int lineNumber;
	const LexerBase *lexer;

	TokenBase() : lexer(0), lineNumber(0), type((Enum)0) {} 

	TokenBase(Enum val, const LexerBase &lexer, int ln, Slice slice)
		: type(val), lexer(&lexer), lineNumber(ln), slice(slice) { }

	char operator[](int n) const
	{
		return lexer->GetInput()[slice.Start + n];
	}

	std::string Text() const
	{
		if (lexer == 0)
			return "";

		return std::move(lexer->GetLine(lineNumber).substr(slice.Start, slice.Length()));
	}
};

KAI_END

