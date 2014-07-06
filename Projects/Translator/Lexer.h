#pragma once

#include <vector>
#include <map>

#include "Slice.h"
#include "Token.h"
#include "Process.h"

#ifdef KAI_USE_MONOTONIC_ALLOCATOR
#	include <boost/monotonic/monotonic.hpp>
#endif

KAI_TRANS_BEGIN

// Tokenise an input string for later parsing
struct Lexer : Process
{
	//boost::monotonic::string<> input;
	std::string input;

	Lexer(const char *input);

	void Print() const;

private:
	friend struct Token;
	friend struct Parser;

	int offset, lineNumber;

#ifdef KAI_USE_MONOTONIC_ALLOCATOR
	typedef boost::monotonic::vector<Token> Tokens;
	typedef boost::monotonic::vector<std::string> Lines;
	typedef boost::monotonic::map<std::string, Token::Type> Keywords;
#else
	typedef std::vector<Token> Tokens;
	typedef std::vector<std::string> Lines;
	typedef std::map<std::string, Token::Type> Keywords;
#endif

	Lines lines;
	Tokens tokens;
	Keywords keyWords;

	void CreateLines();
	void AddKeywords();
	bool Run();
	char Current() const;
	char Next();
	bool EndOfLine() const;
	char Peek() const;

	bool NextToken();
	bool LexAlpha();
	const std::string &Line() const;

	void SearchForKeyword(Token &tok) const;

	bool Add(Token::Type type, Slice slice);
	bool Add(Token::Type type, int len = 1);

	// consume characters that pass the given filter function
	Slice Gather(int(*filt)(int ch));

	bool AddTwoCharOp(Token::Type ty);
	bool AddIfNext(char ch, Token::Type thenType, Token::Type elseType);
};

KAI_TRANS_END
