#include "KAI/KAI.h"
#include "KAI/Translator/Parser.h"

#include <iostream>
#include <algorithm>
#include <strstream>
#include <stdarg.h>

KAI_BEGIN

using namespace std;

Parser::Parser(std::shared_ptr<Lexer> lexer, Structure st)
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

void Parser::Run(Structure st)
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

bool Parser::Program()
{
	while (!Try(Token::None) && !Failed)
	{
		ConsumeNewLines();
		Statement(root);
	}

	return true;
}

void Parser::Function(NodePtr node)
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

Parser::NodePtr Parser::NewNode(Node::Type t)
{
	return std::make_shared<Node>(t);
}

Parser::NodePtr Parser::NewNode(Token const &t)
{
	return std::make_shared<Node>(t);
}

void Parser::Block(NodePtr node)
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

bool Parser::Statement(NodePtr block)
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

std::shared_ptr<Node> Parser::Expect(Token::Type type)
{
	Token tok = Current();
	if (tok.type != type)
	{
		Fail(Lexer::CreateErrorMessage(tok, "Expected %s, have %s", Token::ToString(type), Token::ToString(tok.type)));
		KAI_THROW_1(LogicError, "Unexpected token");
	}

	Next();
	return std::make_shared<Node>(Last());
}

bool Parser::Try(Token::Type type)
{
	return Current().type == type;
}

Token const &Parser::Consume()
{
	return tokens[current++];
}

bool Parser::PeekIs(Token::Type ty) const
{
	return Peek().type == ty;
}

Token const & Parser::Peek() const
{
	return tokens[current + 1];
}

Token const & Parser::Current() const
{
	return tokens[current];
}

Token const &Parser::Last()
{
	return tokens[current - 1];
}

Token const &Parser::Next()
{
	return tokens[++current];
}

bool Parser::PushConsume()
{
	Push(NewNode(Consume()));
	return true;
}

bool Parser::Expression()
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

bool Parser::Logical()
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

bool Parser::Relational()
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

bool Parser::Additive()
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

bool Parser::Term()
{
	if (!Factor())
		return false;

	while (Try(Token::Mul) || Try(Token::Divide))
	{
		auto node = NewNode(Consume());
		node->Add(Pop());
		if (!Factor())
			return CreateError("Factor expected");

		node->Add(Pop());
		Push(node);
	}

	return true;
}

bool Parser::Factor()
{
	bool paran = Try(Token::OpenParan);
	if (paran)
	{
		Consume();
		if (!Expression())
		{
			CreateError("You can do better");
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

Parser::NodePtr Parser::Pop()
{
	if (stack.empty())
	{
		CreateError("Internal Error: Parse stack empty");
		KAI_THROW_0(EmptyStack);
	}

	auto last = stack.back();
	stack.pop_back();

	return last;
}

void Parser::Push(NodePtr node)
{
	if (node)
		stack.push_back(node);
}

void Parser::Print()
{
	Print(*root, 0);
}

void Parser::Print(Node const &node, int level)
{
	std::cout << Lead(level) << node << std::endl;

	for (auto ch : node.Children)
		Print(*ch, level + 1);
}

std::string Parser::Lead(int level)
{
	return std::move(std::string(level*4, ' '));
}

//warning C4127: conditional expression is constant
#pragma warning (disable:4127)

bool Parser::ParseFactorIdent()
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

void Parser::ParseMethodCall()
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
				Fail("Expression expected after comma");

			args->Add(Pop());
		}
	}

	Push(call);
	Expect(Token::CloseParan);

	if (Try(Token::Replace))
		call->Add(Consume());
}

void Parser::ParseGetMember()
{
	Consume();
	auto get = NewNode(Node::GetMember);
	get->Add(Pop());
	get->Add(Expect(Token::Ident));
	Push(get);
}

void Parser::IfCondition(NodePtr block)
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

void Parser::ParseIndexOp()
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

void Parser::For(NodePtr block)
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

void Parser::While(NodePtr block)
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

bool Parser::CreateError(const char *text)
{
	return Fail(Lexer::CreateErrorMessage(Current(), text));
}

void Parser::AddBlock(NodePtr fun)
{
	auto block = NewNode(Node::Block);
	Block(block);
	fun->Add(block);
}

void Parser::ConsumeNewLines()
{
	while (Try(Token::NewLine))
		Consume();
}

KAI_END
