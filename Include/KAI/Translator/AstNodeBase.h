#pragma once

#include "KAI/Translator/Common.h"

KAI_BEGIN

// A common AST Node, given the enumeration type Enum that
// indicates what type of AST not it is
template <class Token, class AstEnumType>
struct AstNodeBase
{
	typedef Token Token;
	typedef AstNodeBase<Token, AstEnumType> AstNode;
	typedef std::shared_ptr<AstNode> AstNodePtr;
	typedef typename AstEnumType::Enum Enum;
	typedef AstNodeBase<Token, AstEnumType> Self;
	typedef std::vector<std::shared_ptr<Self> > ChildrenType;

	Enum type;
	Token token;
	ChildrenType Children;

	AstNodeBase() { }
	AstNodeBase(Enum e) : type(e) { }
	AstNodeBase(Enum e, Token &t) : type(e), token(t) { }
	AstNodeBase(Token const &t) : type(AstEnumType::TokenType), token(t) { }

	void Add(Token const &tok)
	{
		Add(std::make_shared<Self>(tok));
	}

	void Add(AstNodePtr node)
	{
		Children.push_back(node);
	}

	std::string ToString() const
	{
		std::stringstream out;
		out << "[AstNode " << KAI_NAMESPACE(ToString(type)) << ", token:" << token << "]" << std::ends;
		return std::move(out.str());
	}

	std::string Text() const
	{
		return std::move(token.Text());
	}

	friend std::ostream &operator<<(std::ostream &out, Self const &node)
	{
		return out << node.ToString();
	}
};

KAI_END
