#pragma once

#include <vector>
#include <map>

#include "KAI/Translator/Slice.h"
#include "KAI/Translator/Process.h"
#include "KAI/Translator/LexerBase.h"

#ifdef KAI_USE_MONOTONIC_ALLOCATOR
#	include <boost/monotonic/monotonic.hpp>
#endif

KAI_BEGIN

// Tokenise an input string for later parsing
template <class TokenType>
struct LexerCommon : LexerBase
{
	typedef TokenType Token;
	typedef typename TokenType::Enum EnumType;

	LexerCommon(const char *input);
private:
	//friend struct Token;
	//friend struct Parser;

#ifdef KAI_USE_MONOTONIC_ALLOCATOR
	typedef boost::monotonic::vector<Token> Tokens;
	typedef boost::monotonic::vector<std::string> Lines;
	typedef boost::monotonic::map<std::string, Token::Type> Keywords;
#else
	typedef std::vector<Token> Tokens;
	typedef std::map<std::string, EnumType> Keywords;
#endif
	Tokens tokens;
	Keywords keyWords;

	void AddKeyWords();// override;

	void AddStringToken(int lineNumber, Slice slice) override
	{
		// MUST
		///tokens.push_back(Token(Token::String, *this, lineNumber, slice));
	}

	void LexErrorBase(const char *msg) override
	{
		LexError(msg);
	}

	void Terminate() override;
	bool NextToken() override;

	void LexError(const char *msg);
	bool LexAlpha();
	void SearchForKeyword(Token &tok) const;

	bool Add(EnumType type, Slice slice);
	bool Add(EnumType type, int len = 1);

	bool AddTwoCharOp(EnumType ty);
	bool AddIfNext(char ch, EnumType thenType, EnumType elseType);
	
	void Print() const;

public:
	static std::string CreateErrorMessage(Token tok, const char *fmt, ...);
};

KAI_END
