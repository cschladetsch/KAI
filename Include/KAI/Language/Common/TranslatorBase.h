#pragma once

#include "KAI/Language/Common/TranslatorCommon.h"

KAI_BEGIN

template <class EParser>
struct TranslatorBase : TranslatorCommon
{
	typedef EParser Parser;
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

		auto lex = std::make_shared<Lexer>(text, _reg);
		lex->Process();
		if (lex->GetTokens().empty())
			return Object();
		if (lex->Failed)
			Fail(lex->Error);

		if (trace)
			KAI_TRACE_1(lex->Print());

		auto parse = std::make_shared<Parser>(_reg);
		parse->Process(lex, st);
		if (parse->Failed)
			Fail(parse->Error);

		if (trace)
			KAI_TRACE_1(parse->PrintTree());

		PushNew();
		TranslateNode(parse->GetRoot());

		if (stack.empty())
			KAI_THROW_0(EmptyStack);

		if (trace)
			KAI_TRACE_1(stack.back());

		// TODO: don't want fudge around fact that the entire sequence is wrapped around
		// a root continuation
		Continuation const &root = ConstDeref<Continuation>(stack.back());
		if (trace)
			KAI_TRACE_1(root);

		auto inner = root.GetCode()->At(0);
		if (trace)
			KAI_TRACE_1(inner);

		return inner;
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


