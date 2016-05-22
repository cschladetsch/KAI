#include "Tau/TauToken.h"

KAI_BEGIN

const char *TauTokenEnumType::ToString(Enum val)
{
	switch (val)
	{
		#define CASE(N) case TauTokens::N : return #N;
		#define CASE_LOWER(N) case TauTokens::N : return ToLower(#N);
		#define CASE_REPLACE(N, M) case TauTokens::N : return M;

		CASE(None)
		CASE(Whitespace)
		CASE(Semi)
		CASE(Int)
		CASE(Float )
		CASE(String )
		CASE(True )
		CASE(False)
		CASE(Return)
		CASE(Ident)
		CASE(Dot)
		CASE(Comma)
		CASE(If )
		CASE(Else)
		CASE(For )
		CASE(While)
		CASE(OpenBrace )
		CASE(CloseBrace)
		CASE(OpenParan )
		CASE(CloseParan)
		CASE(Plus )
		CASE(Minus)
		CASE(Mul)
		CASE(Divide)
		CASE(Assign)
		CASE(Less )
		CASE(Equiv)
		CASE(NotEquiv)
		CASE(Greater)
		CASE(LessEquiv)
		CASE(GreaterEquiv)
		CASE(Not)
		CASE(And)
		CASE(Or)
		CASE(Xor)
		CASE(OpenSquareBracket)
		CASE(CloseSquareBracket)
		CASE(Increment)
		CASE(Decrement)
		CASE(BitAnd)
		CASE(BitOr)
		CASE(BitXor)
		CASE(Self)
		CASE(Lookup)
		CASE(Tab)
		CASE(NewLine)
		CASE(Fun)
		CASE(Comment)
		CASE(Yield)
		CASE(Suspend)
		CASE(Replace)
		CASE(Resume)
		CASE(PlusAssign)
		CASE(MinusAssign)
		CASE(MulAssign)
		CASE(DivAssign)
		CASE(In)
		CASE(Assert)
		CASE(ToPi)
		CASE(PiSequence)
		CASE(Debug)
	}

	return "Unnamed";
}

std::ostream &operator<<(std::ostream &out, TauToken const &node)
{
	if (node.type == TauTokenEnumType::None)
		return out;

	out << TauTokenEnumType::ToString(node.type);
	switch (node.type)
	{
	case TauTokenEnumType::Int:
	case TauTokenEnumType::String:
	case TauTokenEnumType::Ident:
		out << "=" << node.Text();
	}

	return out;
}

KAI_END
