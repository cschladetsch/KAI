#include "KAI/ExecutorPCH.h"

KAI_BEGIN

template <class Token>
const char *RhoNode<Token>::ToString(Token::Type ty)
{
	switch (ty)
	{
	case None: return "None";
	case Program: return "Program";
	case Ident: return "Ident";
	case GetMember: return "GetMember";
	case Assignment: return "Assignment";
	case Call: return "Call";
	case Increment: return "Increment";
	case Decrement: return "Decrement";
	case TokenType: return "->";
	case Positive: return "Positive";
	case Negative: return "Negative";
	case Function: return "Function";
	case Block: return "Block";
	case Conditional: return "If";
	case ArgList: return "ArgList";
	case IndexOp: return "Index";
	case List: return "List";
	case ForEach: return "ForEach";
	case For: return "For";
	}
	return "???";
}

KAI_END
