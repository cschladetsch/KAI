#include "KAI/KAI.h"
#include "KAI/Translator/Translator.h"
#include <boost/lexical_cast.hpp>

KAI_BEGIN

Translator::Translator(std::shared_ptr<Parser> p, Registry &r) 
	: reg(r)
{
	PushNew();

	try
	{
		Traverse(p->root);
	}
	catch (Exception &)
	{
		if (!Failed)
			Fail("Failed");
	}
}

void Translator::TranslateFromToken(Parser::NodePtr node)
{
	switch (node->token.type)
	{
	case Token::Less:
		TranslateBinaryOp(node, Operation::Less);
		return;
	case Token::Minus:
		TranslateBinaryOp(node, Operation::Minus);
		return;
	case Token::Plus:
		TranslateBinaryOp(node, Operation::Plus);
		return;
	case Token::Mul:
		TranslateBinaryOp(node, Operation::Multiply);
		return;
	case Token::Divide:
		TranslateBinaryOp(node, Operation::Divide);
		return;
	case Token::Or:
		TranslateBinaryOp(node, Operation::LogicalOr);
		return;
	case Token::And:
		TranslateBinaryOp(node, Operation::LogicalAnd);
		return;
	case Token::Int:
		Append(reg.New<int>(boost::lexical_cast<int>(node->token.Text())));
		return;
	case Token::Float:
		Append(reg.New<float>(boost::lexical_cast<float>(node->token.Text())));
		return;
	case Token::String:
		Append(reg.New<String>(node->token.Text()));
		return;
	case Token::Ident:
		Append(reg.New<Label>(Label(node->token.Text())));
		return;
	}
	Fail("Unsupported node %s (token %s)", Node::ToString(node->type), Token::ToString(node->token.type));
	throw Unsupported();;
}

void Translator::TranslateBinaryOp(Parser::NodePtr node, Operation::Type op)
{
	Translate(node->Children[0]);
	Translate(node->Children[1]);

	AppendNew<Operation>(Operation(op));
}

void Translator::Translate(Parser::NodePtr node)
{
	switch (node->type)
	{
	case Node::IndexOp:
		TranslateBinaryOp(node, Operation::Index);
		return;
	case Node::GetMember:
		TranslateBinaryOp(node, Operation::GetProperty);
		return; 
	case Node::TokenType:
		TranslateFromToken(node);
		return;
	case Node::Assignment:
		TranslateBinaryOp(node, Operation::Assign);
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
	//Node::ChildrenType const &ch = node->Children;

	//// write the body
	//result << " {";
	//for (auto b : ch[2]->Children)
	//	Traverse(b);
	//result << " }";

	//// write the args
	//result << " [";
	//for (auto a : ch[1]->Children)
	//	Traverse(a);
	//result << " ]";

	//// write the name and store
	//result << ' ' << ch[0]->token.Text() << " #fun";
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

	AppendNew(Operation(Operation::Suspend));
}

Pointer<Continuation> Translator::Top()
{
	return stack.back();
}

void Translator::PushNew()
{
	Pointer<Continuation> c = reg.New<Continuation>();
	c->SetCode(reg.New<Array>());
	stack.push_back(c);
}

void Translator::Append(Object ob)
{
	Top()->GetCode()->Append(ob);
}

Pointer<Continuation> Translator::Pop()
{
	auto top = Top();
	stack.pop_back();
	return top;
}

std::string Translator::Result() const
{
	StringStream str;
	for (auto ob : *stack.back()->GetCode())
		str << ' ' << ob;
	return str.ToString().c_str();
}

KAI_END

