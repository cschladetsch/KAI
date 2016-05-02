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
	using typename Parent::Token;
	using typename Parent::TokenEnum;
	using typename Parent::AstNodePtr;
	typedef PiAstNodeEnumType NodeType;
	typedef PiTokenEnumType TokenType;

	PiParser(std::shared_ptr<Lexer> lexer) : Parent(lexer) { }

	virtual void Process(Structure st) override;

protected:

private:
	bool Compound();

	Object NewObject();
	bool NextSingle();
	bool ParseArray();
	bool ParseContinuation();
};

KAI_END

