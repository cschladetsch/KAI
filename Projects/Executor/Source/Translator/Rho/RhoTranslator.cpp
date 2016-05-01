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

void RhoTranslator::TranslateFromToken(AstNodePtr node)
{
	switch (node->type)
	{
	case TokenType::While:
		TranslateWhile(node);
		return;

	case TokenType::DivAssign:
		TranslateBinaryOp(node, Operation::DivEquals);
		return;

	case TokenType::MulAssign:
		TranslateBinaryOp(node, Operation::MulEquals);
		return;

	case TokenType::MinusAssign:
		TranslateBinaryOp(node, Operation::MinusEquals);
		return;

	case TokenType::PlusAssign:
		TranslateBinaryOp(node, Operation::PlusEquals);
		return;

	case TokenType::Assign:
		TranslateBinaryOp(node, Operation::Store);
		return;

	case TokenType::Lookup:
		AppendNewOp(Operation::Lookup);
		return;

	case TokenType::Self:
		AppendNewOp(Operation::This);
		return;

	case TokenType::NotEquiv:
		TranslateBinaryOp(node, Operation::NotEquiv);
		return;

	case TokenType::Equiv:
		TranslateBinaryOp(node, Operation::Equiv);
		return;

	case TokenType::Less:
		TranslateBinaryOp(node, Operation::Less);
		return;

	case TokenType::Greater:
		TranslateBinaryOp(node, Operation::Greater);
		return;

	case TokenType::GreaterEquiv:
		TranslateBinaryOp(node, Operation::GreaterOrEquiv);
		return;

	case TokenType::LessEquiv:
		TranslateBinaryOp(node, Operation::LessOrEquiv);
		return;

	case TokenType::Minus:
		TranslateBinaryOp(node, Operation::Minus);
		return;

	case TokenType::Plus:
		TranslateBinaryOp(node, Operation::Plus);
		return;

	case TokenType::Mul:
		TranslateBinaryOp(node, Operation::Multiply);
		return;

	case TokenType::Divide:
		TranslateBinaryOp(node, Operation::Divide);
		return;

	case TokenType::Or:
		TranslateBinaryOp(node, Operation::LogicalOr);
		return;

	case TokenType::And:
		TranslateBinaryOp(node, Operation::LogicalAnd);
		return;

	case TokenType::Int:
		Append(reg.New<int>(boost::lexical_cast<int>(node->token.Text())));
		return;

	case TokenType::Float:
		Append(reg.New<float>(boost::lexical_cast<float>(node->token.Text())));
		return;

	case TokenType::String:
		Append(reg.New<String>(node->Text()));
		return;

	case TokenType::Ident:
		Append(reg.New<Label>(Label(node->Text())));
		return;

	case TokenType::Yield:
		//for (auto ch : node->Children)
		//	Translate(ch);
		//AppendNewOp(Operation::PushContext);
		KAI_NOT_IMPLEMENTED();
		return;

	case TokenType::Return:
		for (auto ch : node->Children)
			Translate(ch);
		AppendNewOp(Operation::Return);
		return;
	}

	//MUST
	//Fail("Unsupported node %s (token %s)", RhoAstNode::ToString(node->type), Token::ToString(node->token.type));
	throw Unsupported();
}

void RhoTranslator::TranslateBinaryOp(AstNodePtr node, Operation::Type op)
{
	Translate(node->Children[0]);
	Translate(node->Children[1]);

	AppendNew<Operation>(Operation(op));
}

void RhoTranslator::Translate(AstNodePtr node)
{
	if (!node)
	{
		Failed = true;
		return;
	}

	switch (node->type)
	{
	case AstEnum::IndexOp:
		TranslateBinaryOp(node, Operation::Index);
		return;

	case AstEnum::GetMember:
		TranslateBinaryOp(node, Operation::GetProperty);
		return;

	case AstEnum::TokenType:
		TranslateFromToken(node);
		return;

	case AstEnum::Assignment:
		// like a binary op, but argument order is reversed
		Translate(node->Children[1]);
		Translate(node->Children[0]);
		AppendNew<Operation>(Operation(Operation::Store));
		return;

	case AstEnum::Call:
		TranslateCall(node);
		return;

	case AstEnum::Conditional:
		TranslateIf(node);
		return;

	case AstEnum::Block:
		PushNew();
		for (auto st : node->Children)
			Translate(st);
		Append(Pop());
		return;

	case AstEnum::List:
		for (auto ch : boost::adaptors::reverse(node->Children))
			Translate(ch);
		AppendNew<int>(node->Children.size());
		AppendNewOp(Operation::ToArray);
		return;

	case AstEnum::For:
		TranslateFor(node);
		return;

	case AstEnum::Function:
		TranslateFunction(node);
		return;

	case AstEnum::Program:
		for (auto e : node->Children)
			Translate(e);
		return;
	}

	//MUST Fail("Unsupported node %s (token %s)", AstNode::ToString(node->type), RhoTokenEnumType::ToString(node->token.type));
	throw Unsupported();
}

void RhoTranslator::TranslateBlock(AstNodePtr node)
{
	for (auto st : node->Children)
		Translate(st);
}

void RhoTranslator::TranslateFunction(AstNodePtr node)
{
	// child 0: ident
	// child 1: args
	// child 2: block
	AstNode::ChildrenType const &ch = node->Children;

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
	AppendNew(Label(ch[0]->Text()));
	AppendNewOp(Operation::Store);
}

void RhoTranslator::TranslateCall(AstNodePtr node)
{
	typename AstNode::ChildrenType &children = node->Children;
	for (auto a : children[1]->Children)
		Translate(a);

	Translate(children[0]);
	if (children.size() > 2 && children[2]->token.type == TokenType::Replace)
		AppendNew(Operation(Operation::Replace));
	else
		AppendNew(Operation(Operation::SuspendNew));
}

Pointer<Continuation> RhoTranslator::Top()
{
	return stack.back();
}

void RhoTranslator::PushNew()
{
	Pointer<Continuation> c = reg.New<Continuation>();
	c->SetCode(reg.New<Array>());
	stack.push_back(c);
}

void RhoTranslator::Append(Object ob)
{
	Top()->GetCode()->Append(ob);
}

Pointer<Continuation> RhoTranslator::Pop()
{
	auto top = Top();
	stack.pop_back();
	return top;
}

std::string RhoTranslator::Result() const
{
	StringStream str;
	for (auto ob : *stack.back()->GetCode())
		str << ' ' << ob;

	return str.ToString().c_str();
}

void RhoTranslator::AppendNewOp(Operation::Type op)
{
	AppendNew<Operation>(Operation(op));
}

void RhoTranslator::TranslateIf(AstNodePtr node)
{
	typename AstNode::ChildrenType const &ch = node->Children;
	bool hasElse = ch.size() > 2;
	Translate(ch[0]);
	if (hasElse)
		Translate(ch[2]);

	Translate(ch[1]);
	AppendNewOp(hasElse ? Operation::IfThenSuspendElseSuspend : Operation::IfThenSuspend);
}

void RhoTranslator::TranslateFor(AstNodePtr node)
{
	AppendNewOp(Operation::None);
}

void RhoTranslator::TranslateWhile(AstNodePtr node)
{
	AppendNewOp(Operation::None);
}

KAI_END

