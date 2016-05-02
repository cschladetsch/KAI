#pragma once

#include "KAI/Translator/Common.h"

KAI_BEGIN

template <class Parser>
struct TranslatorBase : Process
{
	typedef typename Parser::Token Token;
	typedef typename Parser::Lexer Lexer;
	typedef typename Parser::AstNode AstNode;
	typedef typename Parser::TokenNode TokenNode;
	typedef typename AstNode::Enum AstEnum;
	typedef typename TokenNode::Enum TokenEnum;
	typedef typename Parser::AstNodePtr AstNodePtr;
	//typedef typename Parer::

	TranslatorBase(const TranslatorBase&) = delete;
	TranslatorBase(std::shared_ptr<Parser> p, Registry &reg);

	std::string TextResult() const;
	Pointer<Continuation> Result() const { return Top(); }

private:
	virtual void Traverse(AstNodePtr node) = 0;
	virtual void Translate(AstNodePtr node) = 0;
	virtual void TranslateFunction(AstNodePtr node) = 0;
	virtual void TranslateBlock(AstNodePtr node) = 0;
	virtual void TranslateBinaryOp(AstNodePtr node, typename Operation::Type) = 0;
	virtual void TranslateFromToken(AstNodePtr node) = 0;
	virtual void TranslateCall(AstNodePtr node) = 0;
	virtual void TranslateIndex(AstNodePtr node) = 0;

private:
	Registry &reg;
	std::vector<Pointer<Continuation>> stack;

	Pointer<Continuation> Top() const;
	void PushNew();
	void Append(Object ob);
	template <class T>
	void AppendNew(T const &val)
	{
		Append(reg.New<T>(val));
	}
	template <class T>
	void AppendNew()
	{
		Append(reg.New<T>());
	}

	Pointer<Continuation> Pop();
	void AppendNewOp(Operation::Enum op);
	void TranslateIf(AstNodePtr node);
	void TranslateFor(AstNodePtr node);
	void TranslateWhile(AstNodePtr node);
};

KAI_END


