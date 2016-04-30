#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/Rho/Rho.h"

#include <boost/lexical_cast.hpp>
#include <boost/range/adaptor/reversed.hpp>

KAI_BEGIN

RhoTranslator::RhoTranslator(std::shared_ptr<RhoParser> p, Registry &r)
	: reg(r)
{
	if (p->Failed)
		return;

	PushNew();

	try
	{
		Translate(p->GetRoot());
	}
	catch (Exception &)
	{
		if (!Failed)
			Fail("Failed");
	}
}

void RhoTranslator::TranslateFromToken(NodePtr node)
{
	switch (node->type)
	{
	case RhoToken::While:
		TranslateWhile(node);
		return;

	case RhoToken::DivAssign:
		TranslateBinaryOp(node, Operation::DivEquals);
		return;

	case RhoToken::MulAssign:
		TranslateBinaryOp(node, Operation::MulEquals);
		return;

	case RhoToken::MinusAssign:
		TranslateBinaryOp(node, Operation::MinusEquals);
		return;

	case RhoToken::PlusAssign:
		TranslateBinaryOp(node, Operation::PlusEquals);
		return;

	case RhoToken::Assign:
		TranslateBinaryOp(node, Operation::Store);
		return;

	case RhoToken::Lookup:
		AppendNewOp(Operation::Lookup);
		return;

	case RhoToken::Self:
		AppendNewOp(Operation::This);
		return;

	case RhoToken::NotEquiv:
		TranslateBinaryOp(node, Operation::NotEquiv);
		return;

	case RhoToken::Equiv:
		TranslateBinaryOp(node, Operation::Equiv);
		return;

	case RhoToken::Less:
		TranslateBinaryOp(node, Operation::Less);
		return;

	case RhoToken::Greater:
		TranslateBinaryOp(node, Operation::Greater);
		return;

	case RhoToken::GreaterEquiv:
		TranslateBinaryOp(node, Operation::GreaterOrEquiv);
		return;

	case RhoToken::LessEquiv:
		TranslateBinaryOp(node, Operation::LessOrEquiv);
		return;

	case RhoToken::Minus:
		TranslateBinaryOp(node, Operation::Minus);
		return;

	case RhoToken::Plus:
		TranslateBinaryOp(node, Operation::Plus);
		return;

	case RhoToken::Mul:
		TranslateBinaryOp(node, Operation::Multiply);
		return;

	case RhoToken::Divide:
		TranslateBinaryOp(node, Operation::Divide);
		return;

	case RhoToken::Or:
		TranslateBinaryOp(node, Operation::LogicalOr);
		return;

	case RhoToken::And:
		TranslateBinaryOp(node, Operation::LogicalAnd);
		return;

	case RhoToken::Int:
		Append(reg.New<int>(boost::lexical_cast<int>(node->token.Text())));
		return;

	case RhoToken::Float:
		Append(reg.New<float>(boost::lexical_cast<float>(node->token.Text())));
		return;

	case RhoToken::String:
		Append(reg.New<String>(node->token.Text()));
		return;

	case RhoToken::Ident:
		Append(reg.New<Label>(Label(node->token.Text())));
		return;

	case RhoToken::Yield:
		//for (auto ch : node->Children)
		//	Translate(ch);
		//AppendNewOp(Operation::PushContext);
		KAI_NOT_IMPLEMENTED();
		return;

	case RhToken::Return:
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
	if (!node)
	{
		Failed = true;
		return;
	}

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
		for (auto ch : boost::adaptors::reverse(node->Children))
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

	case Node::Program:
		for (auto e : node->Children)
			Translate(e);
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

void Translator::TranslateFunction(NodePtr node)
{
	// child 0: ident
	// child 1: args
	// child 2: block
	Node::ChildrenType const &ch = node->Children;

	// write the body
	PushNew();
	for (auto b : ch[2]->Children)
		Translate(b);

	// add the args
	auto cont = Pop();
	for (auto a : ch[1]->Children)
		cont->AddArg(Label(a->token.Text()));

	// write the name and store
	Append(cont);
	AppendNew(Label(ch[0]->token.Text()));
	AppendNewOp(Operation::Store);
}

void Translator::TranslateCall(NodePtr node)
{
	Node::ChildrenType &children = node->Children;
	for (auto a : children[1]->Children)
		Translate(a);

	Translate(children[0]);
	if (children.size() > 2 && children[2]->token.type == Token::Replace)
		AppendNew(Operation(Operation::Replace));
	else
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

