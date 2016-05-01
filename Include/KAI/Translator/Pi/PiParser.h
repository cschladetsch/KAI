#pragma once

#include "KAI/Translator/TokenBase.h"
#include "KAI/Translator/AstNodeBase.h"
#include "KAI/Translator/ParserCommon.h"
#include "KAI/Translator/Structure.h"
#include "KAI/Translator/Pi/PiAstNode.h"

KAI_BEGIN

// parser specific to the in-fix Pi language
struct PiParser : ParserCommon<PiLexer, PiAstNodeEnumType>
{
	typedef ParserCommon<PiLexer, PiAstNodeEnumType> Parent;
	using typename Parent::Lexer;
	using typename Parent::AstNode;
	using typename Parent::Token;
	using typename Parent::TokenEnum;
	using typename Parent::AstNodePtr;
	typedef PiAstNodeEnumType NodeType;
	typedef PiTokenEnumType TokenType;

	PiParser(std::shared_ptr<Lexer> lexer) : Parent(lexer) { }
	PiParser(std::shared_ptr<Lexer> lexer, Structure st) : Parent(lexer) { Run(st); }

	bool Program();
};
