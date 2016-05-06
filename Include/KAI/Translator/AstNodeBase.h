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
	AstNodeBase(Enum e) : _astType(e) { }
	AstNodeBase(Enum e, Token &t) : _astType(e), _token(t) { }
	AstNodeBase(Token const &t) : _astType(AstEnumType::TokenType), _token(t) { }

	const Child &GetChild(size_t n) const { return GetChildren()[n]; }
	const ChildrenType &GetChildren() const { return _children; }

	Enum GetType() const { return _astType; }
	const Token &GetToken() const { return _token; }
	std::string GetTokenText() const { return std::move(_token.Text()); }
	Enum GetEnum() const { return _astType; }

	std::string ToString() const
	{
		std::stringstream out;
#ifdef KAI_TRACE_VERBOSE
		out << "(AstNode " << KAI_NAMESPACE(ToString(type)) << ", token:" << token << ")";
#else
		out << KAI_NAMESPACE(ToString(_astType)) << ": " << _token.ToString();
#endif
		return out.str();
	}

	std::string Text() const
	{
		return std::move(_token.Text());
	}

	friend std::ostream &operator<<(std::ostream &out, Self const &node)
	{
		return out << node.ToString();
	}

	void Add(AstNodePtr node)
	{
		_children.push_back(node);
	}

	void Add(Token const &tok)
	{
		Add(std::make_shared<Self>(tok));
	}

protected:
	Enum _astType;
	Token _token;
	ChildrenType _children;
	Object Object;
};

KAI_END
