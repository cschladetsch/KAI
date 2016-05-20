#pragma once

#include <KAI/Core/Config/Base.h>
#include <KAI/Language/Common/TokenBase.h>
#include "TauAstNode.h"

KAI_BEGIN

struct TauTokenEnumType
{
	enum Enum
	{
		None = 0,
		Ident = 1,
		OpenParan = 2,
		CloseParan = 3,
		Value = 4,
		Const = 5,
		Reference = 9,
		Proxy = 11,
		Agent = 12,
		Semi = 13,
		NewLine = 14,
		OpenBrace = 15,
		CloseBrace = 16,
		Module = 17,
		Namespace = 18,
		ConstProxy = 19,
		ConstAgent = 20,
		Int = 21,
		Float = 22,
		String = 23,
		Void = 24,
		Bool = 25,
		ArrayProxy = 27,
		QuotedIdent = 28,
		Array = 29,
		Container = 30,
	};

	struct Type : TokenBase<TauTokenEnumType>
	{
		Type() { }

		Type(Enum val, const LexerBase &lexer, int ln, Slice slice)
				: TokenBase<TauTokenEnumType>(val, lexer, ln, slice) { }
	};

	static const char *ToString(Enum val);
};

typedef TauTokenEnumType::Type TauToken;


KAI_END
