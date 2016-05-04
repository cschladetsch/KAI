#pragma once

#include "KAI/Translator/Common.h"
#include "KAI/Translator/Pi/PiAstNode.h"
#include "KAI/Translator/Pi/PiLexer.h"

KAI_BEGIN

// parser specific to the in-fix Pi language
struct PiParser : ParserCommon<PiLexer, PiAstNodeEnumType>
{
	typedef ParserCommon<PiLexer, PiAstNodeEnumType> Parent;
	using typename Parent::Lexer;
	using typename Parent::AstNode;
	using typename Parent::TokenNode;
	using typename Parent::TokenEnum;
	using typename Parent::AstNodePtr;
	typedef PiAstNodeEnumType NodeType;
	typedef PiTokenEnumType TokenType;

	PiParser(Registry &r) : Parent(r) { }

	virtual void Process(std::shared_ptr<Lexer> lex, Structure st) override;

private:
	bool NextSingle(AstNodePtr context);
	bool ParseArray(AstNodePtr context);
	bool ParseContinuation(AstNodePtr context);
};

KAI_END

