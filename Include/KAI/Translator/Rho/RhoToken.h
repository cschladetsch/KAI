#pragma once

#include "KAI/Translator/Common.h"

KAI_BEGIN

// A token in the language
struct RhoTokenEnumType
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
		Plus,
		Minus,
		Mul,
		Divide,
		Assign,
		Less,
		Equiv,
		NotEquiv,
		Greater,
		LessEquiv,
		GreaterEquiv,
		Not,
		And,
		Or,
		Xor,
		OpenSquareBracket,
		CloseSquareBracket,
		Increment,
		Decrement,
		BitAnd,
		BitOr,
		BitXor,
		Self,
		Lookup,
		Tab,
		NewLine,
		Fun,
		Comment,
		Yield,
		Suspend,
		Replace,
		Resume,
		PlusAssign,
		MinusAssign,
		MulAssign,
		DivAssign,
		In,
		Assert,
		ToPi,
		ToPiSequence,
	};

	struct Type : TokenBase<RhoTokenEnumType>
	{
		Type() { }

		Type(Enum val, const LexerBase &lexer, int ln, Slice slice) 
			: TokenBase<RhoTokenEnumType>(val, lexer, ln, slice) { }
	};
};

typedef RhoTokenEnumType::Type RhoToken;
typedef RhoTokenEnumType RhoTokens;

const char *ToString(RhoTokenEnumType::Enum val);

KAI_END
