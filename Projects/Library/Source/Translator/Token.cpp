#include "KAI/KAI.h"
#include "KAI/Translator/Token.h"
#include "KAI/Translator/Lexer.h"

KAI_BEGIN

Token::Token(Type type, const Lexer &lexer, int ln, Slice slice) 
	: type(type), lexer(&lexer), lineNumber(ln), slice(slice)
{
}

char Token::operator[](int n) const
{
	return lexer->input[slice.Start + n];
}

std::string Token::Text() const
{
	if (lexer == 0)
		return "";

	return std::move(lexer->lines[lineNumber].substr(slice.Start, slice.Length()));
}

const char * Token::ToString(Type t)
{
	switch (t)
	{
	case None: return "None";
	case Whitespace: return "";
	case Semi: return "Semi";
	case Int: return "Int";
	case Float: return "Float";
	case String: return "String";
	case Ident: return "Ident";
	case Dot: return "Dot";
	case If: return "If";
	case Else: return "Else";
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
	case Return: return "Return";
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
	case Fun: return "Fun";
	case Tab: return "Tab";
	case NewLine: return "NewLine";
	case Comment: return "Comment";
	case PlusAssign: return "PlusAssign";
	case MinusAssign: return "MinusAssign";
	case MulAssign: return "MulAssign";
	case DivAssign: return "DivAssign";
	case Yield: return "Yield";
	}

	static char b[100];
	_itoa_s(t, b, 100, 10);
	return b;
}

std::ostream &operator<<(std::ostream &out, Token const &node)
{
	if (node.type == Token::None)
		return out;

	out << Token::ToString(node.type);
	switch (node.type)
	{
	case Token::Int:
	case Token::String:
	case Token::Ident:
		out << "=" << node.Text();
	}

	return out;
}

KAI_END
