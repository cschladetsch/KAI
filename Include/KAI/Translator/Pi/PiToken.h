#pragma once

#include "KAI/Translator/Common.h"

KAI_BEGIN

// A token in the language
struct PiTokenEnumType
{
	enum Enum
	{
		None, Null = None,
		Whitespace,
		Int, Float, String,
		True, False,
		Suspend, Resume, Replace,
		Ident,
		Dot,
		Comma,
		If, IfElse,
		For, While,
		OpenBrace, CloseBrace,
		OpenParan, CloseParan,
		Plus, Minus, Mul, Divide,
		Assign,
		Less, Equiv, NotEquiv, Greater, LessEquiv, GreaterEquiv,
		Not, And, Or, Xor,
		OpenSquareBracket, CloseSquareBracket,
		Increment, Decrement,
		BitAnd, BitOr, BitXor,
		Self,
		Lookup,
		Tab, NewLine,
		Comment,
		PlusAssign, MinusAssign, MulAssign, DivAssign,
		In, Colon,
		Assert,
		Pathname,

		// RPN specific
		Dup,
		Drop,
		Over,
		OverM,
		PickN,
		Depth,
		Swap,
		Rot,
		RotN,
		Clear,
		Expand,
		ToArray, ToList, ToMap, ToSet, ToHashMap, ToPair, ToVector2, ToVector3, ToVector4, ToQuaternion,
		RunScript,
		Name, Fullname,
		GetProperty,
		SetProperty,
		GarbageCollect,
		ChangeFolder,
		PrintFolder,
		Pipe,
		DirectIn,
		DirectOut,
		DirectInOut,
		GetPath,
		GetName,
		SplitPathName,
		MakePathName,
		Setname,
		SetChild,
		GetChild,
		GetChildren,
		GetMethods,
		GetProperties,
		GetType,
		GetSignature,
		StringtoType,
		TypeToString,

		Version,

		DateNowSimple,
		DateNow,
		DurationSinceStart,
		DeltaTime,

		PushFolder,
		PopFolder,
		History,

		Jobs,

	};

	struct Type : TokenBase<PiTokenEnumType>
	{
		Type() { }
		Type(Enum val, const LexerBase &lexer, int ln, Slice slice) 
			: TokenBase<PiTokenEnumType>(val, lexer, ln, slice) { }

		friend std::ostream &operator<<(std::ostream &out, Type const &node);
	};
};

typedef PiTokenEnumType PiTokens;
typedef PiTokenEnumType::Type PiToken;

static const char *ToString(PiTokens::Enum t);

KAI_END

