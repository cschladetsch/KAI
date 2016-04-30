#pragma once

#include <string>

#include "KAI/Translator/Common.h"

KAI_BEGIN

template <class Token>
struct LexerCommon;

// A token in the language
struct RhoToken
{
	enum Enum
	{
		None,
		Whitespace,
		Semi,
		Int,
		Float,
		String,
		True,
		False,
		Return,
		Ident,
		Dot,
		Comma,
		If,
		Else,
		For,
		While,
		OpenBrace,
		CloseBrace,
		OpenParan,
		CloseParan,
		Plus, Minus, Mul, Divide,
		Assign,
		Less, Equiv, NotEquiv, Greater, LessEquiv, GreaterEquiv,

		Not, And, Or, Xor,

		OpenSquareBracket, CloseSquareBracket,
		Increment, Decrement,
		BitAnd, BitOr, BitXor,

		Self,
		Lookup,

		// added because we want whitespace to matter for formatting, as in python
		Tab, NewLine,
		Fun,

		Comment,

		Yield,
		
		Suspend, Replace, Resume,
		PlusAssign, MinusAssign, MulAssign, DivAssign,
		In, Colon,
		Assert,

	};

	struct Node : TokenBase<Enum>
	{
		friend std::ostream &operator<<(std::ostream &out, Node const &node)
		{
			//return out << ToString(node.type) << ": " << node.token;
			return out << "token-node";
		}
	};
};

KAI_END

