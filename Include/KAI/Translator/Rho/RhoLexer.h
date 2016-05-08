#pragma once

#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/Rho/RhoToken.h"

KAI_BEGIN

struct RhoLexer : LexerCommon<RhoTokenEnumType>
{
	typedef LexerCommon<RhoTokenEnumType> Parent;
	typedef TokenBase<RhoTokenEnumType> TokenNode;

	RhoLexer(const char *text) : Parent(text) { }

	virtual void AddKeyWords();
	virtual bool NextToken();
	virtual void Terminate();
};

KAI_END

