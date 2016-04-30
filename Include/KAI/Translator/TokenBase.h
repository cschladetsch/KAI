#pragma once

KAI_BEGIN

// common to all Tokens
struct TokenBase
{
	const LexerBase *lexer;
	Slice slice;
	int lineNumber;
	std::string Text() const;
	char operator[](int n) const;
};

KAI_END

