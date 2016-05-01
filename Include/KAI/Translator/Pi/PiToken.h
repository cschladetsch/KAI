#pragma once

#include "KAI/Translator/Common.h"

KAI_BEGIN

// A token in the language
struct PiTokenEnumType
{
	enum Enum
	{
		None,
		Whitespace,
		Int,
		Float,
		String,
		True,
		False,
		Suspend,
		Resume,
		Replace,
		Ident,
		Dot,
		Comma,
		If,
		IfElse,
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
		Comment,
		PlusAssign, MinusAssign, MulAssign, DivAssign,
		In, Colon,
		Assert,
	};

	static const char *ToString(Enum t);

	class Type : TokenBase<PiTokenEnumType>
	{
		Type() { }
		Type(Enum val, const LexerBase &lexer, int ln, Slice slice) 
			: TokenBase<PiTokenEnumType>(val, lexer, ln, slice) { }

		friend std::ostream &operator<<(std::ostream &out, Type const &node);
	};
};

typedef PiTokenEnumType::Type PiToken;

KAI_END

