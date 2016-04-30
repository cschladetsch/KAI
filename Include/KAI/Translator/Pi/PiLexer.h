#pragma once

#include <vector>
#include <map>

#include "KAI/Translator/Slice.h"
#include "KAI/Translator/Process.h"
#include "KAI/Translator/Pi/Token.h"

#ifdef KAI_USE_MONOTONIC_ALLOCATOR
#	include <boost/monotonic/monotonic.hpp>
#endif

KAI_PI_BEGIN

// Tokenise an input string for later parsing
template <class Token>
struct PiLexer : Lexer
{
	//boost::monotonic::string<> input;
	PiLexer(const char *input);

	void Print() const;

private:
	friend struct PiToken;
	friend struct PiParser;

#ifdef KAI_USE_MONOTONIC_ALLOCATOR
	typedef boost::monotonic::vector<Token> Tokens;
	typedef boost::monotonic::vector<std::string> Lines;
	typedef boost::monotonic::map<std::string, EnumType> Keywords;
#else
	typedef std::vector<Token> Tokens;
	typedef std::vector<std::string> Lines;
	typedef std::map<std::string, EnumType> Keywords;
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

	void LexError(const char *msg);

	bool LexAlpha();
	const std::string &Line() const;

	void SearchForKeyword(Token &tok) const;

	bool Add(EnumType type, Slice slice);
	bool Add(EnumType type, int len = 1);

	// consume characters that pass the given filter function
	Slice Gather(int(*filt)(int ch));

	bool AddTwoCharOp(EnumType ty);
	bool AddIfNext(char ch, EnumType thenType, EnumType elseType);
	
public:
	static std::string CreateErrorMessage(Token tok, const char *fmt, ...);
	bool LexString();
};

KAI_PI_END
