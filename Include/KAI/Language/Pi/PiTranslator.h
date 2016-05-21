#pragma once

#include "KAI/Language/Common/TranslatorBase.h"
#include "KAI/Language/Pi/PiToken.h"
#include "KAI/Language/Pi/PiLexer.h"
#include "KAI/Language/Pi/PiAstNode.h"
#include "KAI/Language/Pi/PiParser.h"

KAI_BEGIN

class PiTranslator : public TranslatorBase<PiParser>
{
public:
	typedef TranslatorBase<PiParser> Parent;
	typedef typename Parent::TokenNode TokenNode;
	typedef typename Parent::AstNode AstNode;
	typedef typename Parent::AstNodePtr AstNodePtr;

	PiTranslator() = delete;
	PiTranslator(Registry &r) : Parent(r) { }
	virtual Pointer<Continuation> Result() override
	{
		return stack.front();
	}

protected:
	virtual void TranslateNode(AstNodePtr node) override;

private:
	void AppendTokenised(const TokenNode& tok);
};

KAI_END
