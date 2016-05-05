#pragma once

#include "KAI/Translator/Common.h"
#include "KAI/HierarchicalPrinter.h"

KAI_BEGIN

// A common AST Node, given the enumeration type Enum that
// indicates what type of AST not it is
template <class Token, class AstEnumType>
class AstNodeBase : public HierarchicalPrinter<AstNodeBase<Token, AstEnumType> >
{
public:
	typedef Token Token;
	typedef AstNodeBase<Token, AstEnumType> AstNode;
	typedef std::shared_ptr<AstNode> AstNodePtr;
	typedef typename AstEnumType::Enum Enum;
	typedef AstNodeBase<Token, AstEnumType> Self;
	typedef std::shared_ptr<Self> Child;
	typedef std::vector<Child > ChildrenType;

	AstNodeBase() { }
	AstNodeBase(Enum e) : type(e) { }
	AstNodeBase(Enum e, Token &t) : type(e), token(t) { }
	AstNodeBase(Token const &t) : type(AstEnumType::TokenType), token(t) { }
	AstNodeBase(Object Q) : type(AstEnumType::Object), Object(Q) { }

	const Child &GetChild(size_t n) const { return GetChildren()[n]; }
	const ChildrenType &GetChildren() const { return Children; }

	Enum GetType() const { return type; }
	const Token &GetToken() const { return token; }
	std::string GetTokenText() const { return std::move(token.Text()); }
	Enum GetEnum() const { return type; }

	std::string ToString() const
	{
		std::stringstream out;
#ifdef KAI_TRACE_VERBOSE
		out << "(AstNode " << KAI_NAMESPACE(ToString(type)) << ", token:" << token << ")" << std::ends;
#else
		out << "( " << KAI_NAMESPACE(ToString(type)) << ": " << KAI_NAMESPACE(ToString(token.type)) << ") " << std::ends;
#endif
		return out.str();
	}

	std::string Text() const
	{
		return std::move(token.Text());
	}

	friend std::ostream &operator<<(std::ostream &out, Self const &node)
	{
		return out << node.ToString();
	}

	void Add(AstNodePtr node)
	{
		Children.push_back(node);
	}

	void Add(Token const &tok)
	{
		Add(std::make_shared<Self>(tok));
	}

protected:
	Enum type;
	Token token;
	ChildrenType Children;

	Object Object;
};

KAI_END
