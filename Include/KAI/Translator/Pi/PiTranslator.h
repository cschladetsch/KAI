#pragma once

#include "KAI/KAICommon.h"
#include "KAI/Translator/TranslatorBase.h"
#include "KAI/Translator/Pi/PiToken.h"
#include "KAI/Translator/Pi/PiLexer.h"
#include "KAI/Translator/Pi/PiAstNode.h"
#include "KAI/Translator/Pi/PiParser.h"

KAI_BEGIN

struct PiTranslator : TranslatorBase<PiLexer>
{
	typedef TranslatorBase<PiLexer> Parent;
	typedef typename Parent::Token Token;
	typedef typename Parent::AstNode AstNode;
	typedef typename Parent::TokenEnum TokenEnum;
	typedef typename Parent::AstEnum AstNodeEnum;
	typedef typename Parent::AstNodePtr AstNodePtr;
	

};

KAI_END