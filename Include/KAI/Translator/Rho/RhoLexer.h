#pragma once

#include "KAI/ExecutorPCH.h"

KAI_BEGIN

struct RhoLexer : LexerCommon<RhoTokenEnumType>
{
	typedef LexerCommon<RhoTokenEnumType> Parent;

	RhoLexer(const char *text) : Parent(text)
	{
	}

	virtual void AddKeyWords();
	virtual bool NextToken();
	virtual void Terminate();
};

KAI_END

