#pragma once

#include "KAI/KAICommon.h"
#include "KAI/Translator/TranslatorBase.h"
#include "KAI/Translator/Pi/PiToken.h"
#include "KAI/Translator/Pi/PiLexer.h"
#include "KAI/Translator/Pi/PiAstNode.h"
#include "KAI/Translator/Pi/PiParser.h"

KAI_BEGIN

struct PiTranslator : TranslatorBase<PiLexer, PiTokens>
{
	typedef RhoAstNodeEnumType::Node AstNode;
	typedef RhoToken Token;
	typedef LexerCommon<Token> Lexer;
	typedef ParserCommon<Lexer, AstNode> Parser;
	typedef typename RhoParser::AstNodePtr AstNodePtr;
	typedef typename AstNode::Enum AstEnum;
	typedef RhoTokenEnumType TokenType;
	typedef RhoAstNodeEnumType AstType;

};

KAI_END