#pragma once

#include "KAI/KAICommon.h"
#include "KAI/Translator/TranslatorBase.h"
#include "KAI/Translator/Pi/PiToken.h"
#include "KAI/Translator/Pi/PiLexer.h"
#include "KAI/Translator/Pi/PiAstNode.h"
#include "KAI/Translator/Pi/PiParser.h"

KAI_BEGIN

struct PiTranslator : TranslatorBase<PiParser>
{
	typedef TranslatorBase<PiParser> Parent;
	typedef typename Parent::TokenNode TokenNode;
	typedef typename Parent::AstNode AstNode;
	typedef typename Parent::TokenEnum TokenEnum;
	typedef typename Parent::AstEnum AstNodeEnum;
	typedef typename Parent::AstNodePtr AstNodePtr;
	
private:
	virtual void Traverse(AstNodePtr node) override;
	virtual void Translate(AstNodePtr node) override;
};

KAI_END