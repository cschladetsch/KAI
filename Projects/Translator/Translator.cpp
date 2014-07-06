#include "Precompiled.h"
#include "Translator.h"

KAI_TRANS_BEGIN

Translator::Translator(Parser const *p)
{
	try
	{
		Traverse(p->root);
	}
	catch (Exception &)
	{
		if (!Failed)
			Fail("Failed");
	}
	result << std::ends;
}

void Translator::TranslateFromToken(Parser::NodePtr node)
{
	switch (node->token.type)
	{
	case Token::Less:
		TranslateBinaryOp(node, "<");
		return;
	case Token::Minus:
		TranslateBinaryOp(node, "-");
		return;
	case Token::Plus:
		TranslateBinaryOp(node, "+");
		return;
	case Token::Mul:
		TranslateBinaryOp(node, "mul");
		return;
	case Token::Divide:
		TranslateBinaryOp(node, "div");
		return;
	case Token::Or:
		TranslateBinaryOp(node, "or");
		return;
	case Token::And:
		TranslateBinaryOp(node, "and");
		return;
	case Token::Int:
	case Token::String:
	case Token::Float:
	case Token::Ident:
		AddText(node);
		return;
	}
	Fail("Unsupported node %s (token %s)", Node::ToString(node->type), Token::ToString(node->token.type));
	throw Unsupported();;
}

std::string Translator::ConvertOp(Parser::NodePtr node)
{
	switch (node->token.type)
	{
	case Token::Mul: return "mul";
	case Token::Divide: return "div";
	}
	return node->token.Text();
}

void Translator::TranslateBinaryOp(Parser::NodePtr node, std::string op)
{
	Translate(node->Children[0]);
	Translate(node->Children[1]);

	result << ' ' << op;
}

void Translator::Translate(Parser::NodePtr node)
{
	switch (node->type)
	{
	case Node::IndexOp:
		BinaryOp(node, "[]");
		return;
	case Node::GetMember:
		BinaryOp(node, ".");
		return; 
	case Node::TokenType:
		TranslateFromToken(node);
		return;
	case Node::Assignment:
		TranslateBinaryOp(node, "#");
		return;
	case Node::Call:
		TranslateCall(node);
		return;
	}
	Fail("Unsupported node %s (token %s)", Node::ToString(node->type), Token::ToString(node->token.type));
	throw Unsupported();
}

void Translator::TranslateBlock(NodePtr node)
{
	// TODO: scoping
	for (auto st : node->Children)
		Translate(st);
}

// 0: ident
// 1: args
// 2: block
void Translator::TranslateFunction(NodePtr node)
{
	Node::ChildrenType const &ch = node->Children;

	// write the body
	result << " {";
	for (auto b : ch[2]->Children)
		Traverse(b);
	result << " }";

	// write the args
	result << " [";
	for (auto a : ch[1]->Children)
		Traverse(a);
	result << " ]";

	// write the name and store
	result << ' ' << ch[0]->token.Text() << " #fun";
}

void Translator::Traverse(NodePtr node)
{
	switch (node->type)
	{
	case Node::Program:
		if (!node->Children.empty())
			Traverse(node->Children[0]);
		break;
	case Node::Function:
		TranslateFunction(node);
		break;
	case Node::Block:
		TranslateBlock(node);
		break;
	default:
		Translate(node);
		break;
	}
}

void Translator::TranslateCall(NodePtr node)
{
	for (auto a : node->Children[1]->Children)
		Translate(a);

	Translate(node->Children[0]);
	result << " &";
}

void Translator::BinaryOp(NodePtr node, const char *op)
{
	Translate(node->Children[0]);
	Translate(node->Children[1]);
	result << ' ' << op;
}

void Translator::AddText(Parser::NodePtr node)
{
	result << ' ' << node->token.Text();
}

KAI_TRANS_END

