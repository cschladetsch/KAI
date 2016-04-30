#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/RhoParser.h"

#include <iostream>
#include <algorithm>
#include <strstream>
#include <stdarg.h>

KAI_BEGIN

template <typename L, class N>
RhoParser<L,N>::RhoParser(std::shared_ptr<Lexer> lexer, Structure st)
{
	current = 0;
	indent = 0;

	if (lexer->Failed)
		return;

	// strip whitespace and comments
	for (auto tok : lexer->tokens)
		if (tok.type != Token::Whitespace && tok.type != Token::Comment)
			tokens.push_back(tok);

	root = NewNode(Node::Program);

	try
	{
		Run(st);
	}
	catch (Exception::Base &e)
	{
		if (!Failed)
			Fail(Lexer::CreateErrorMessage(Current(), "%s", e.ToString()));
	}
	catch (std::exception &f)
	{
		if (!Failed)
			Fail(Lexer::CreateErrorMessage(Current(), "%s", f.what()));
	}
	catch (...)
	{
		if (!Failed)
			Fail(Lexer::CreateErrorMessage(Current(), "internal error"));
	}
}

template <class L, class N>
void RhoParser<L,N>::Run(Structure st)
{
	switch (st)
	{
	case ParseStatement:
		if (!Statement(root))
		{
			CreateError("Statement expected");
			return;
		}
		break;

	case ParseExpression:
		if (!Expression())
		{
			CreateError("Expression expected");
			return;
		}
		root->Add(Pop());
		break;

	case ParseFunction:
		Function(root);
		break;

	case ParseProgram:
		Program();
		break;
	}

	if (!stack.empty())
		Fail("Internal error: Stack not empty after parsing");
}

template <class L, class N>
bool RhoParser<L,N>::Program()
{
	while (!Try(Token::None) && !Failed)
	{
		ConsumeNewLines();
		Statement(root);
	}

	return true;
}

template <class L, class N>
void RhoParser<L,N>::Function(NodePtr node)
{
	ConsumeNewLines();

	Expect(Token::Fun);
	Expect(Token::Ident);
	auto name = Last();
	auto fun = NewNode(Node::Function);
	fun->Add(name);
	Expect(Token::OpenParan);
	auto args = NewNode(Node::None);
	fun->Add(args);

	if (Try(Token::Ident))
	{
		args->Add(Consume());
		while (Try(Token::Comma))
		{
			Consume();
			args->Add(Expect(Token::Ident));
		}
	}

	Expect(Token::CloseParan);
	Expect(Token::NewLine);
	
	AddBlock(fun);
	node->Add(fun);
}

template <class L, class N>
void RhoParser<L,N>::Block(NodePtr node)
{
	ConsumeNewLines();

	++indent;
	while (!Failed)
	{
		int level = 0;
		while (Try(Token::Tab))
		{
			++level;
			Consume();
		}

		if (Try(Token::NewLine))
		{
			Consume();
			continue;
		}

		// close current block
		if (level < indent)
		{
			--indent;

			// rewind to start of tab sequence to determine next block
			--current;
			while (Try(Token::Tab))
				--current;

			++current;
			return;
		}

		if (level != indent)
		{
			Fail(Lexer::CreateErrorMessage(Current(), "Mismatch block indent"));
			return;
		}

		Statement(node);
	}
}

template <class L, class N>
bool RhoParser<L,N>::Statement(NodePtr block)
{
	switch (Current().type)
	{
		case Token::Assert:
		{
			Consume();
			if (!Expression())
			{
				Fail(Lexer::CreateErrorMessage(Current(), "Assert needs an expression to test"));
				return false;
			}

			auto ass = NewNode(Consume());
			ass->Add(Pop());
			Push(ass);
			goto finis;
		}

		case Token::Return:
		case Token::Yield:
		{
			auto ret = NewNode(Consume());
			if (Expression())
				ret->Add(Pop());
			block->Add(ret);
			goto finis;
		}
		
		case Token::While:
		{
			While(block);
			return true;
		}

		case Token::For:
		{
			For(block);
			return true;
		}

		case Token::If:
		{
			IfCondition(block);
			return true;
		}
		
		case Token::Fun:
		{
			Function(block);
			return true;
		}
	}

	if (!Expression())
		return false;

	block->Add(Pop());

finis:
	// statements can end with an optional semi followed by a new line
	if (Try(Token::Semi))
		Consume();

	Expect(Token::NewLine);

	return true;
}

template <class L, class N>
bool RhoParser<L,N>::Expression()
{
	bool paran = Try(Token::OpenParan);
	if (paran)
	{
		Consume();
		if (!Expression())
			return false;

		Expect(Token::CloseParan);
		return true;
	}

	if (!Logical())
		return false;

	if (Try(Token::Assign) || Try(Token::PlusAssign) || Try(Token::MinusAssign) || Try(Token::MulAssign) || Try(Token::DivAssign))
	{
		auto node = NewNode(Consume());
		auto ident = Pop();
		if (!Logical())
		{
			Fail(Lexer::CreateErrorMessage(Current(), "Assignment requires an expression"));
			return false;
		}

		node->Add(Pop());
		node->Add(ident);
		Push(node);
	}

	return true;
}

template <class L, class N>
bool RhoParser<L,N>::Logical()
{
	if (!Relational())
		return false;

	while (Try(Token::And) || Try(Token::Or))
	{
		auto node = NewNode(Consume());
		node->Add(Pop());
		if (!Relational())
			return Fail(Lexer::CreateErrorMessage(Current(), "Relational expected"));

		node->Add(Pop());
		Push(node);
	}

	return true;
}

template <class L, class N>
bool RhoParser<L,N>::Relational()
{
	if (!Additive())
		return false;

	while (Try(Token::Less) || Try(Token::Greater) || Try(Token::Equiv) || Try(Token::NotEquiv)
		|| Try(Token::LessEquiv) || Try(Token::GreaterEquiv))
	{
		auto node = NewNode(Consume());
		node->Add(Pop());
		if (!Additive())
			return CreateError("Additive expected");

		node->Add(Pop());
		Push(node);
	}

	return true;
}

template <class L, class N>
bool RhoParser<L,N>::Additive()
{
	// unary +/- operator
	if (Try(Token::Plus) || Try(Token::Minus))
	{
		auto ty = Consume().type;
		Consume();
		if (!Term())
			return CreateError("Term expected");

		auto node = new Node(ty == Token::Plus ? Node::Positive : Node::Negative);
		node->Add(Pop());
		return true;
	}

	if (!Term())
		return false;

	while (Try(Token::Plus) || Try(Token::Minus))
	{
		auto node = NewNode(Consume());
		node->Add(Pop());
		if (!Term())
			return CreateError("Term expected");

		node->Add(Pop());
		Push(node);
	}

	return true;
}

template <class L, class N>
bool RhoParser<L,N>::Term()
{
	if (!Factor())
		return false;

	while (Try(Token::Mul) || Try(Token::Divide))
	{
		auto node = NewNode(Consume());
		node->Add(Pop());
		if (!Factor())
			return CreateError("Factor expected with a term");

		node->Add(Pop());
		Push(node);
	}

	return true;
}

template <class L, class N>
bool RhoParser<L,N>::Factor()
{
	if (Try(Token::OpenParan))
	{
		Consume();
		if (!Expression())
		{
			CreateError("Expected an expression for a factor in parenthesis");

			return false;
		}

		Expect(Token::CloseParan);
		return true;
	}

	if (Try(Token::OpenSquareBracket))
	{
		auto list = NewNode(Node::List);
		do
		{
			Consume();
			if (Expression())
				list->Add(Pop());
			else
				list->Add(0);
		}
		while (Try(Token::Comma));

		Expect(Token::CloseSquareBracket);
		Push(list);

		return true;
	}

	if (Try(Token::Int) || Try(Token::Float) || Try(Token::String) || Try(Token::True) || Try(Token::False))
		return PushConsume();

	if (Try(Token::Self))
		return PushConsume();

	while (Try(Token::Lookup))
		PushConsume();

	if (Try(Token::Ident))
		return ParseFactorIdent();

	return false;
}

//warning C4127: conditional expression is constant
#pragma warning (disable:4127)

template <class L, class N>
bool RhoParser<L,N>::ParseFactorIdent()
{
	PushConsume();

	while (true)
	{
		if (Try(Token::Dot))
		{
			ParseGetMember();
			continue;
		}

		if (Try(Token::OpenParan))
		{
			ParseMethodCall();
			continue;
		}

		if (Try(Token::OpenSquareBracket))
		{
			ParseIndexOp();
			continue;
		}

		break;
	}

	return true;
}

template <class L, class N>
void RhoParser<L,N>::ParseMethodCall()
{
	Consume();
	auto call = NewNode(Node::Call);
	call->Add(Pop());
	auto args = NewNode(Node::ArgList);
	call->Add(args);

	if (Expression())
	{
		args->Add(Pop());
		while (Try(Token::Comma))
		{
			Consume();
			if (!Expression())
			{
				Fail("What is the next argument?");
				return;
			}

			args->Add(Pop());
		}
	}

	Push(call);
	Expect(Token::CloseParan);

	if (Try(Token::Replace))
		call->Add(Consume());
}

template <class L, class N>
void RhoParser<L,N>::ParseGetMember()
{
	Consume();
	auto get = NewNode(Node::GetMember);
	get->Add(Pop());
	get->Add(Expect(Token::Ident));
	Push(get);
}

template <class L, class N>
void RhoParser<L,N>::IfCondition(NodePtr block)
{
	if (!Try(Token::If))
		return;

	Consume();

	// get the test expression
	if (!Expression())
	{
		CreateError("If what?");
		return;
	}

	NodePtr condition = Pop();
	//Expect(Token::CloseParan);

	// get the true-clause
	NodePtr trueClause = NewNode(Node::Block);
	Block(trueClause);

	// make the conditional node in AST
	NodePtr cond = NewNode(Node::Conditional);
	cond->Add(condition);
	cond->Add(trueClause);

	// if there's an else, add it as well
	if (Try(Token::Else))
	{
		Consume();
		NodePtr falseClause = NewNode(Node::Block);
		Block(falseClause);
		cond->Add(falseClause);
	}

	block->Add(cond);
}

template <class L, class N>
void RhoParser<L,N>::ParseIndexOp()
{
	Consume();
	auto index = NewNode(Node::IndexOp);
	index->Add(Pop());
	if (!Expression())
	{
		CreateError("Index what?");
		return;
	}

	Expect(Token::CloseSquareBracket);
	index->Add(Pop());
	Push(index);
}

template <class L, class N>
void RhoParser<L,N>::For(NodePtr block)
{
	if (!Try(Token::For))
		return;

	Consume();

	auto f = NewNode(Node::For);
	if (!Expression())
	{
		CreateError("For what?");
		return;
	}

	if (Try(Token::In))
	{
		Consume();
		f->Add(Pop());

		if (!Expression())
		{
			CreateError("For each in what?");
			return;
		}

		f->Add(Pop());
	}
	else
	{
		Expect(Token::Semi);
		f->Add(Pop());

		if (!Expression())
		{
			CreateError("When does the for statement stop?");
			return;
		}

		f->Add(Pop());
		Expect(Token::Semi);

		if (!Expression())
		{
			CreateError("What happens when a for statement ends?");
			return;
		}

		f->Add(Pop());
	}

	Expect(Token::NewLine);
	AddBlock(f);
	block->Add(f);
}

template <class L, class N>
void RhoParser<L,N>::While(NodePtr block)
{
	auto w = NewNode(Consume());
	if (!Expression())
	{
		CreateError("While what?");
		return;
	}

	w->Add(Pop());
	block->Add(w);
}

template <class L, class N>
bool RhoParser<L,N>::CreateError(const char *text)
{
	return Fail(Lexer::CreateErrorMessage(Current(), text));
}

template <class L, class N>
void RhoParser<L,N>::AddBlock(NodePtr fun)
{
	auto block = NewNode(Node::Block);
	Block(block);
	fun->Add(block);
}

template <class L, class N>
void RhoParser<L,N>::ConsumeNewLines()
{
	while (Try(Token::NewLine))
		Consume();
}
KAI_END
