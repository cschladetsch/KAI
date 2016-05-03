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

};

KAI_END