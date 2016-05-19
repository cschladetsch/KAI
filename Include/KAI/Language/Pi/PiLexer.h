#pragma once

#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/Pi/PiToken.h"

KAI_BEGIN

class PiLexer : public LexerCommon<PiTokenEnumType>
{
public:
	typedef LexerCommon<PiTokenEnumType> Parent;
	typedef Parent Lexer;
	typedef TokenBase<PiTokenEnumType> TokenNode;

	PiLexer(const char *text) : Parent(text) { }

	virtual void AddKeyWords();
	virtual bool NextToken();
	virtual void Terminate();

private:
	bool QuotedIdent();
};

KAI_END
