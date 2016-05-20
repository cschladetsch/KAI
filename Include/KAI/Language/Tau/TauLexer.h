#pragma once

#include <KAI/Language/Common/LexerCommon.h>
#include "KAI/Language/Tau/TauToken.h"

KAI_BEGIN

struct TauLexer : LexerCommon<TauTokenEnumType>
{
	typedef LexerCommon<TauTokenEnumType> Parent;
	typedef TokenBase<TauTokenEnumType> TokenNode;

	TauLexer(const char *text) : Parent(text) { }

	virtual void AddKeyWords();
	virtual bool NextToken();
	virtual void Terminate();
};

KAI_END

