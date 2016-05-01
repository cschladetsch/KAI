#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/Pi/PiToken.h"

KAI_BEGIN

const char *PiTokenEnumType::ToString(PiTokenEnumType::Enum t)
{
	switch (t)
	{
	case None: return "None";
	case Whitespace: return "";
	case Int: return "Int";
	case Float: return "Float";
	case String: return "String";
	case Ident: return "Ident";
	case Dot: return "Dot";
	case If: return "If";
	case For: return "For";
	case While: return "While";
	case OpenParan: return "OpenParan";
	case CloseParan: return "CloseParan";
	case Plus:  return "Plus";
	case Minus:  return "Minus";
	case Mul:  return "Mul";
	case Divide: return "Divide";
	case Assign: return "Assign";
	case Less:  return "Less";
	case Equiv:  return "Equiv";
	case Greater:  return "Greater";
	case LessEquiv:  return "LessEquiv";
	case GreaterEquiv: return "GreaterEqiv";
	case OpenBrace: return "OpenBrace";
	case CloseBrace: return "CloseBrace";
	case Not: return "Not";
	case NotEquiv: return "NotEquiv";
	case And: return "And";
	case Or: return "Or";
	case Comma: return "Comma";
	case OpenSquareBracket: return "OpenSquareBracket";
	case CloseSquareBracket: return "CloseSquareBracket";
	case Increment: return "++";
	case Decrement: return "--";
	case Self: return "Self";
	case Lookup: return "Lookup";
	case Tab: return "Tab";
	case NewLine: return "NewLine";
	case Comment: return "Comment";
	case PlusAssign: return "PlusAssign";
	case MinusAssign: return "MinusAssign";
	case MulAssign: return "MulAssign";
	case DivAssign: return "DivAssign";
	}

	static char b[100];
	_itoa_s(t, b, 100, 10);
	return b;
}

std::ostream &operator<<(std::ostream &out, PiToken const &node)
{
	if (node.type == PiTokenEnumType::None)
		return out;

	out << PiTokenEnumType::ToString(node.type);
	switch (node.type)
	{
	case PiTokenEnumType::Int:
	case PiTokenEnumType::String:
	case PiTokenEnumType::Ident:
		out << "=" << node.Text();
	}

	return out;
}

KAI_END
