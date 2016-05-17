#pragma once

#include "KAI/KAI.h"
#include "KAI/Translator/Common.h"
#include "KAI/HierarchicalPrinter.h"
#include "Language.h"

KAI_BEGIN

// A common AST Node, given the enumeration type Enum that
// indicates what type of AST not it is
template <class EToken, class AstEnumType>
class AstNodeBase : public HierarchicalPrinter<AstNodeBase<EToken, AstEnumType> >
{
public:
	typedef EToken Token;
	typedef AstNodeBase<Token, AstEnumType> AstNode;
	typedef std::shared_ptr<AstNode> AstNodePtr;
	typedef typename AstEnumType::Enum Enum;
	typedef AstNodeBase<Token, AstEnumType> Self;
	typedef std::shared_ptr<Self> Child;
	typedef std::vector<Child > ChildrenType;

	AstNodeBase() { }
	AstNodeBase(Enum e) : _astType(e) { }
	AstNodeBase(Enum e, Token &t) : _astType(e), _token(t) { }
	AstNodeBase(Enum e, Object const &Q) : _astType(e), _object(Q) { }
	AstNodeBase(Token const &t) : _astType(AstEnumType::TokenType), _token(t) { }

	const Child &GetChild(size_t n) const { return GetChildren()[n]; }
	const ChildrenType &GetChildren() const { return _children; }

	Enum GetType() const { return _astType; }
	const Token &GetToken() const { return _token; }
	std::string GetTokenText() const { return std::move(_token.Text()); }
	//Enum GetEnum() const { return _astType; }

	std::string ToString() const
	{
		std::stringstream out;
#ifdef KAI_TRACE_VERBOSE
		out << "(AstNode " << KAI_NAMESPACE(ToString(type)) << ", token:" << token << ")";
#else
		out << (int)_astType << ": " << _token.ToString();
		// TODO can't do this because AstType not defined yetL
		// out << KAI_NAMESPACE(ToString(_astType)) << ": " << _token.ToString();
		return std::move(out.str());
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

	void Add(Enum type, Object content)
	{
		_children.push_back(std::make_shared<Self>(type, content));
	}

	void Add(Token const &tok)
	{
		Add(std::make_shared<Self>(tok));
	}

protected:
	Enum _astType;
	Token _token;
	ChildrenType _children;
	Object _object;
};

KAI_END
