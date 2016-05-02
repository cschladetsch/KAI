#pragma once

#include "KAI/Translator/Common.h"
#include "KAI/Translator/AstNodeBase.h"

KAI_BEGIN

struct RhoAstNodeEnumType
{
	enum Enum : int
	{
		None,
		Program,
		Ident,
		GetMember,
		Function,
		Block,
		Conditional,
		Assignment,
		Call,
		Increment,
		Decrement,
		TokenType,
		Positive,
		Negative,
		ArgList,
		IndexOp,
		ForEach,
		For,
		List,
		Map,
		Plus,
		Minus,
		Mul,
		Divide,
		Less,
		Equiv,
		NotEquiv,
		Greater,
	};

	struct Node : AstNodeBase<RhoToken, RhoAstNodeEnumType>
	{
		typedef AstNodeBase<RhoToken, RhoAstNodeEnumType> Parent;
		using typename Parent::Token;

		Node() : Parent(Enum::None) {  }
		Node(Enum e) : Parent(e) { }
		Node(Enum e, Token &tok) : Parent(e, tok) { }
		Node(Parent::Token tok) : Parent(tok) { }
	};
};

typedef RhoAstNodeEnumType RhoAstNodes;
typedef RhoAstNodes::Node RhoAstNode;

const char *ToString(RhoAstNodes::Enum val);

KAI_END
