#pragma once


#include "KAI/Translator/Pi/PiBase.h"
#include "KAI/Translator/Slice.h"
#include <string>

KAI_PI_BEGIN

template <class Token>
struct Lexer;

// A token in the language
struct PiToken
{
	enum Type
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

	static const char *ToString(Type t);

	Type type;
	Slice slice;
	int lineNumber;
	const Lexer<PiToken> *lexer;

	PiToken() : lexer(0), type(None) { }
	PiToken(Type type, const Lexer<PiToken> &lexer, int lineNumber, Slice slice);

	std::string Text() const;
	char operator[](int n) const;

	friend std::ostream &operator<<(std::ostream &out, PiToken const &node);
};

KAI_PI_END

