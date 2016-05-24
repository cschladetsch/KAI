#pragma once

#include <KAI/Language/Common/LexerCommon.h>
#include "KAI/Language/Pi/PiToken.h"

KAI_BEGIN

class PiLexer : public LexerCommon<PiTokenEnumType>
{
public:
	typedef LexerCommon<PiTokenEnumType> Parent;
	typedef Parent Lexer;
	typedef TokenBase<PiTokenEnumType> TokenNode;

	PiLexer(const char *text, Registry &r) : Parent(text, r) { }

	void AddKeyWords() override;
	bool NextToken() override ;
	void Terminate() override ;

private:
	bool QuotedIdent();

protected:
	using Parent::_reg;

	bool ParsePathname();
};

KAI_END
