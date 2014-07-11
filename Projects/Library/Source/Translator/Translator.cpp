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
	case Token::While:
		TranslateWhile(node);
		return;
	case Token::DivAssign:
		TranslateBinaryOp(node, Operation::DivEquals);
		return;
	case Token::MulAssign:
		TranslateBinaryOp(node, Operation::MulEquals);
		return;
	case Token::MinusAssign:
		TranslateBinaryOp(node, Operation::MinusEquals);
		return;
	case Token::PlusAssign:
		TranslateBinaryOp(node, Operation::PlusEquals);
		return;
	case Token::Assign:
		TranslateBinaryOp(node, Operation::Store);
		return;
	case Token::Lookup:
		AppendNewOp(Operation::Lookup);
		return;
	case Token::Self:
		AppendNewOp(Operation::This);
		return;
	case Token::NotEquiv:
		TranslateBinaryOp(node, Operation::NotEquiv);
		return;
	case Token::Equiv:
		TranslateBinaryOp(node, Operation::Equiv);
		return;
	case Token::Less:
		TranslateBinaryOp(node, Operation::Less);
		return;
	case Token::Greater:
		TranslateBinaryOp(node, Operation::Greater);
		return;
	case Token::GreaterEqiv:
		TranslateBinaryOp(node, Operation::GreaterOrEquiv);
		return;
	case Token::LessEquiv:
		TranslateBinaryOp(node, Operation::LessOrEquiv);
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
	case Token::Yield:
		//for (auto ch : node->Children)
		//	Translate(ch);
		//AppendNewOp(Operation::PushContext);
		//AppendNewOp(Operation::Replace);
		KAI_NOT_IMPLEMENTED();
		return;

	case Token::Return:
		for (auto ch : node->Children)
			Translate(ch);
		AppendNewOp(Operation::Return);
		return;
	}
	Fail("Unsupported node %s (token %s)", Node::ToString(node->type), Token::ToString(node->token.type));
	throw Unsupported();
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
		// like a binary op, but argument order is reversed
		Translate(node->Children[1]);
		Translate(node->Children[0]);
		AppendNew<Operation>(Operation(Operation::Store));
		return;
	case Node::Call:
		TranslateCall(node);
		return;
	case Node::Conditional:
		TranslateIf(node);
		return;
	case Node::Block:
		PushNew();
		for (auto st : node->Children)
			Translate(st);
		Append(Pop());
		return;
	case Node::List:
		for (auto ch : node->Children)
			Translate(ch);
		AppendNew<int>(node->Children.size());
		AppendNewOp(Operation::ToArray);
		return;
	case Node::For:
		TranslateFor(node);
		return;
	case Node::Function:
		TranslateFunction(node);
		return;
	}

	Fail("Unsupported node %s (token %s)", Node::ToString(node->type), Token::ToString(node->token.type));
	throw Unsupported();
}

void Translator::TranslateBlock(NodePtr node)
{
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
	PushNew();
	for (auto b : ch[2]->Children)
		Traverse(b);
	auto cont = Pop();

	// add the args
	for (auto a : ch[1]->Children)
		cont->AddArg(Label(a->token.Text()));

	// write the name and store
	Append(cont);
	AppendNew(Label(ch[0]->token.Text()));
	AppendNewOp(Operation::Store);
}

void Translator::Traverse(NodePtr node)
{
	switch (node->type)
	{
	case Node::Program:
		for (auto ch : node->Children)
			Translate(ch);
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
	AppendNew(Operation(Operation::SuspendNew));
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

void Translator::AppendNewOp(Operation::Type op)
{
	AppendNew<Operation>(Operation(op));
}

void Translator::TranslateIf(Parser::NodePtr node)
{
	Node::ChildrenType const &ch = node->Children;
	bool hasElse = ch.size() > 2;
	Translate(ch[0]);
	if (hasElse)
		Translate(ch[2]);
	Translate(ch[1]);
	AppendNewOp(hasElse ? Operation::IfThenSuspendElseSuspend : Operation::IfThenSuspend);
}

void Translator::TranslateFor(Parser::NodePtr node)
{
	AppendNewOp(Operation::None);
}

void Translator::TranslateWhile(Parser::NodePtr node)
{
	AppendNewOp(Operation::None);
}

KAI_END

