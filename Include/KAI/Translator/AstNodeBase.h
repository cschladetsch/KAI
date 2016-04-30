#pragma once

#include "KAI/Translator/Common.h"

KAI_BEGIN

// A common AST Node, given the enumeration type Enum that
// indicates what type of AST not it is
template <class AstType>
struct AstNodeBase
{
	typedef AstNodeBase<AstType> Self;
	typedef typename AstType::Enum Enum;
	typedef std::vector<std::shared_ptr<Self> > ChildrenType;

	Enum type;
	TokenBase *token;
	ChildrenType Children;

	AstNodeBase(Enum e) : type(e) { }
	AstNodeBase(Enum e, TokenBase &t) : type(e), token(&t) { }

	void Add(TokenBase const &tok)
	{
		Add(std::make_shared<Self>(tok));
	}

	void Add(std::shared_ptr < Self > node)
	{
		Children.push_back(node);
	}

	static const char *ToString(Enum ty);
};

KAI_END


