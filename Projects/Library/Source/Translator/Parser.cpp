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

	// strip whitespace
	for (auto tok : lexer->tokens)
		if (tok.type != Token::Whitespace && tok.type != Token::Comment)
			tokens.push_back(tok);

	root = std::make_shared<Node>(Node::Program);

	try
	{
		Run(st);
	}
	catch (Exception &)
	{
		if (!Failed)
			Fail("Failed");
	}
	catch (std::exception &f)
	{
		if (!Failed)
			Fail(f.what());
	}
}

void Parser::Run(Structure st)
{
	try
	{
		switch (st)
		{
		case ParseStatement:
			Statement(root);
			break;
		case ParseExpression:
			Expression();
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
			Fail("Stack not empty");
	}
	catch (Exception &)
	{
		//cerr << Error << endl;
	}
}

bool Parser::Program()
{
	while (!Try(Token::None) && !Failed)
	{
		while (Try(Token::NewLine))
			Next();

		if (Try(Token::Fun))
			Function(root);
		else
			Statement(root);
	}

	return true;
}

void Parser::Function(NodePtr node)
{
	while (Try(Token::NewLine))
		Consume();

	Expect(Token::Fun);
	Expect(Token::Ident);
	auto name = Last();
	Expect(Token::OpenParan);

	auto fun = NewNode(Node::Function);
	fun->Add(name);
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
	
	auto block = NewNode(Node::Block);
	Block(block);
	fun->Add(block);
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
	while (Try(Token::NewLine))
		Consume();

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
			Fail(Lexer::CreateError(Current(), "Mismatch block indent"));
			continue;
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
				Fail(Lexer::CreateError(Current(), "Assert needs an expression to test"));
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
	if (Try(Token::NewLine))
		Consume();
	else
	{
		Expect(Token::Semi);
		Expect(Token::NewLine);
	}

	return true;
}

std::shared_ptr<Node> Parser::Expect(Token::Type type)
{
	Token tok = Current();
	if (tok.type != type)
	{
		Fail(Lexer::CreateError(tok, "Expected %s, have %s", Token::ToString(type), Token::ToString(tok.type)));
		throw Unexpected(tok.type);
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

void Parser::PushConsume()
{
	Push(NewNode(Consume()));
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
		root->Add(Pop());
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
			Fail(Lexer::CreateError(Current(), "Assignment requires an expression"));
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
		{
			return Fail(Lexer::CreateError(Current(), "expression expected"));
		}
		node->Add(Pop());
		Push(node);
	}

	return true;
}

bool Parser::Relational()
{
	if (!Additive())
		return false;

	while (Try(Token::Less) || Try(Token::Greater) || Try(Token::Equiv) || Try(Token::NotEquiv) || Try(Token::LessEquiv) || Try(Token::GreaterEqiv))
	{
		auto node = NewNode(Consume());
		node->Add(Pop());
		if (!Additive())
		{
			return Fail(Lexer::CreateError(Current(), "expression expected"));
		}
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
		{
			return Fail(Lexer::CreateError(Current(), "term expected"));
		}
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
		{
			Fail(Lexer::CreateError(Current(), "Term needed"));
			return false;
		}
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
		auto node = NewNode(Consume());;
		node->Add(Pop());
		Factor();
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
		Expression();
		Expect(Token::CloseParan);
		return true;
	}

	if (Try(Token::OpenSquareBracket))
	{
		NodePtr list = NewNode(Node::List);
		do
		{
			Consume();
			if (Expression())
				list->Add(Pop());
		} while (Try(Token::Comma));

		Expect(Token::CloseSquareBracket);
		Push(list);
		return true;
	}

	if (Try(Token::Int) || Try(Token::Float) 
		|| Try(Token::String) || Try(Token::True) || Try(Token::False))
	{
		PushConsume();
		return true;
	}

	if (Try(Token::Self))
	{
		PushConsume();
		return true;
	}

	while (Try(Token::Lookup))
	{
		PushConsume();
	}

	if (Try(Token::Ident))
	{
		return ParseFactorIdent();
	}

	return false;
}

Parser::NodePtr Parser::Pop()
{
	if (stack.empty())
	{
		Fail(Lexer::CreateError(Current(), "Element expected"));
		throw StackError();
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
		Print(*ch, level + 2);
}

std::string Parser::Lead(int level)
{
	std::string s = "";
	for (int n = 0; n < level; ++n)
		s += "  ";
	return s;
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
			{
				Fail("Expression expected after comma");
			}
			args->Add(Pop());
		}
	}

	Push(call);
	Expect(Token::CloseParan);
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
	//Expect(Token::OpenParan);
	Expression();
	NodePtr condition = Pop();
	//Expect(Token::CloseParan);

	// get the true-clause
	NodePtr true_clause = NewNode(Node::Block);
	Block(true_clause);

	// make the conditional node in AST
	NodePtr ifCond = NewNode(Node::Conditional);
	ifCond->Add(condition);
	ifCond->Add(true_clause);

	// if there's an else, add it as well
	if (Try(Token::Else))
	{
		Consume();
		NodePtr false_clause = NewNode(Node::Block);
		Block(false_clause);
		ifCond->Add(false_clause);
	}

	block->Add(ifCond);
}

void Parser::ParseIndexOp()
{
	Consume();
	auto index = NewNode(Node::IndexOp);
	index->Add(Pop());
	Expression();
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
	Expression();
	if (Try(Token::In))
	{
		Consume();
		f->Add(Pop());

		Expression();
		f->Add(Pop());
	}
	else
	{
		Expect(Token::Semi);
		f->Add(Pop());

		Expression();
		f->Add(Pop());
		Expect(Token::Semi);

		Expression();
		f->Add(Pop());
	}

	Expect(Token::NewLine);
	auto body = NewNode(Node::Block);
	Block(body);
	f->Add(body);
	block->Add(f);
}

void Parser::While(NodePtr block)
{
	auto w = NewNode(Consume());
	if (!Expression())
	{
		Fail(Lexer::CreateError(Current(), "While requires an expression"));
		return;
	}
	w->Add(Pop());

	auto body = NewNode(Node::Block);
	Block(body);
	w->Add(body);
	block->Add(w);
}

Parser::Unexpected::Unexpected(Token::Type ty)
{
	KAI_UNUSED_1(ty);
}

KAI_END

