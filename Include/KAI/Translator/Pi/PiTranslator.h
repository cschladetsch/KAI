#pragma once

#include "KAI/KAICommon.h"
#include "KAI/Translator/TranslatorBase.h"
#include "KAI/Translator/Pi/PiToken.h"
#include "KAI/Translator/Pi/PiLexer.h"
#include "KAI/Translator/Pi/PiAstNode.h"
#include "KAI/Translator/Pi/PiParser.h"

KAI_BEGIN

class PiTranslator : public TranslatorBase<PiParser>
{
public:
	typedef TranslatorBase<PiParser> Parent;
	typedef typename Parent::Parser Parser;
	typedef typename Parent::TokenNode TokenNode;
	typedef typename Parent::AstNode AstNode;
	typedef typename Parent::TokenEnum TokenEnum;
	typedef typename Parent::AstEnum AstNodeEnum;
	typedef typename Parent::AstNodePtr AstNodePtr;

	PiTranslator() = delete;
	PiTranslator(Registry &r) : Parent(r) { }

	//virtual Pointer<Continuation> Translate(const char *text, Structure st) override;

protected:
	virtual void TranslateNode(AstNodePtr node) override;

private:
	void AppendSingle(const TokenNode& tok);
};

KAI_END