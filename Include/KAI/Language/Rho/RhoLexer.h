#pragma once

#include "KAI/Language/Rho/RhoToken.h"
#include "KAI/Language/Common/LexerCommon.h"

KAI_BEGIN

struct RhoLexer : LexerCommon<RhoTokenEnumType>
{
	typedef LexerCommon<RhoTokenEnumType> Parent;
	typedef TokenBase<RhoTokenEnumType> TokenNode;

	RhoLexer(const char *text, Registry &r) : Parent(text, r) { }

	void AddKeyWords() override;
	bool NextToken() override;
	void Terminate() override;

protected:
	using Parent::_reg;
};

KAI_END

