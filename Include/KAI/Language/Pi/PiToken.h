#pragma once

// TODO: Use kai::StringStringStream
#include <ostream>
#include <KAI/Language/Common/TokenBase.h>

KAI_BEGIN

// A token in the Pi language
struct PiTokenEnumType
{
	enum Enum
	{
		None = 0,
		Whitespace,
		Int,
		Float,
		String,
		Bool,
		True,
		False,
		Suspend,
		Resume,
		Replace,
		Ident,
		QuotedIdent,
		Store,
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
		Comment,
		PlusAssign,
		MinusAssign,
		MulAssign,
		DivAssign,
		In,
		Colon,
		Assert,
		Pathname,
		ToRho,
		ToRhoSequence,
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
		ToArray,
		ToList,
		ToMap,
		ToSet,
		ToHashMap,
		ToPair,
		ToVector2,
		ToVector3,
		ToVector4,
		ToQuaternion,
		RunScript,
		Name,
		Fullname,
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
		SetName,
		SetChild,
		GetChild,
		GetChildren,
		GetMethods,
		GetProperties,
		GetType,
		GetSignature,
		StringToType,
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
		Debug,
		Size,
		Exists,
	};

	struct Type : TokenBase<PiTokenEnumType>
	{
		Type() { }
		Type(Enum val, const LexerBase &lexer, int ln, Slice slice)
			: TokenBase<PiTokenEnumType>(val, lexer, ln, slice) { }

		friend std::ostream &operator<<(std::ostream &out, Type const &node);
	};

	static const char *ToString(Enum t);
};

typedef PiTokenEnumType PiTokens;
typedef PiTokenEnumType::Type PiToken;

KAI_END

