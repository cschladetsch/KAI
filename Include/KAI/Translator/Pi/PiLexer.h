#pragma once

#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/Pi/PiToken.h"

KAI_BEGIN

struct PiLexer : LexerCommon<PiTokenEnumType>
{
	typedef LexerCommon<PiTokenEnumType> Parent;
	PiLexer(const char *text) : Parent(text) { }

	virtual void AddKeyWords();
	virtual bool NextToken();
	virtual void Terminate();
};

KAI_END
