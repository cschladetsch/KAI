#pragma once

#include "KAI/Translator/TranslatorCommon.h"

KAI_BEGIN

template <class Parser>
struct TranslatorBase : TranslatorCommon
{
	typedef typename Parser::TokenNode TokenNode;
	typedef typename TokenNode::Enum TokenEnum;
	typedef typename Parser::Lexer Lexer;
	typedef typename Parser::AstNode AstNode;
	typedef typename AstNode::Enum AstEnum;
	typedef typename Parser::AstNodePtr AstNodePtr;

	TranslatorBase(const TranslatorBase&) = delete;
	TranslatorBase(std::shared_ptr<Parser> p, Registry &reg);

private:
	virtual void Traverse(AstNodePtr node) = 0;
	virtual void Translate(AstNodePtr node) = 0;
};

KAI_END


