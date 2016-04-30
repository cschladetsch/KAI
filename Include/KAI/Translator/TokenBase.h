#pragma once

KAI_BEGIN

// common to all Tokens
template <class Enum>
struct TokenBase
{
	const LexerBase *lexer;
	Enum type;
	Slice slice;
	int lineNumber;
	std::string Text() const;
	char operator[](int n) const;
};

KAI_END

