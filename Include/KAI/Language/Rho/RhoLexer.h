#pragma once

#include "KAI/Language/Rho/RhoToken.h"

KAI_BEGIN

struct RhoLexer : LexerCommon<RhoTokenEnumType>
{
	typedef LexerCommon<RhoTokenEnumType> Parent;
	typedef TokenBase<RhoTokenEnumType> TokenNode;

	RhoLexer(const char *text) : Parent(text) { }

	void AddKeyWords() override ;
	bool NextToken() override ;
	void Terminate() override ;
};

KAI_END

