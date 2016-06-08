#pragma once

#include <KAI/Language/Common/ParserCommon.h>
#include <KAI/Language/Common/Structure.h>
#include <KAI/Language/Pi/PiAstNode.h>
#include <KAI/Language/Pi/PiLexer.h>

KAI_BEGIN

// parser specific to the post-fix Pi language
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
	void Run(Structure);
	bool NextSingle(AstNodePtr context);
	bool ParseArray(AstNodePtr context);
	bool ParseContinuation(AstNodePtr context);
};

KAI_END

