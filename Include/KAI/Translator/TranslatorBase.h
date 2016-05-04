#pragma once

#include "KAI/Translator/TranslatorCommon.h"

KAI_BEGIN

template <class Parser>
struct TranslatorBase : TranslatorCommon
{
	typedef Parser Parser;
	typedef typename Parser::TokenNode TokenNode;
	typedef typename TokenNode::Enum TokenEnum;
	typedef typename Parser::Lexer Lexer;
	typedef typename Parser::AstNode AstNode;
	typedef typename AstNode::Enum AstEnum;
	typedef typename Parser::AstNodePtr AstNodePtr;

	TranslatorBase(const TranslatorBase&) = delete;
	TranslatorBase(Registry &reg) : TranslatorCommon(reg) { }

	virtual Pointer<Continuation> Translate(const char *text, Structure st) override
	{
		if (text == 0 || text[0] == 0)
			return Object();

		auto lex = std::make_shared<Lexer>(text);
		lex->Process();
		if (lex->GetTokens().empty())
			return Object();
		if (lex->Failed)
			Fail(lex->Error);

		auto parse = std::make_shared<Parser>(reg);
		parse->Process(lex, st);
		if (parse->Failed)
			Fail(parse->Error);

		PushNew();
		TranslateNode(parse->GetRoot());

		if (stack.empty())
			KAI_THROW_0(EmptyStack);

		return stack.back();
	}

protected:
	virtual void TranslateNode(AstNodePtr node) = 0;

	void Run(std::shared_ptr<Parser> p)
	{
		PushNew();

		try
		{
			TranslateNode(p->GetRoot());
		}
		catch (Exception &)
		{
			if (!Failed)
				Fail("Failed");
		}
	}
};

KAI_END


