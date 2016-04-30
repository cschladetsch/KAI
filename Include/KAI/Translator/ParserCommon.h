#pragma once

#include "KAI/Translator/LexerCommon.h"
#include "KAI/Translator/AstNodeBase.h"
#include "KAI/Translator/ParserBase.h"

KAI_BEGIN

// common for all parsers.
// iterate over a stream of tokens to produce an abstract syntax tree
template <class Lexer, class Node>
struct ParserCommon : ParserBase<Lexer, Node>
{
	typedef ParserBase<Lexer, Node> Parent;
	using typename Parent::Lexer;
	using typename Parent::Node;
	using typename Parent::Token;
	using typename Parent::TokenEnum;
	//using typename Parent::NodeType;
	using typename Parent::NodePtr;

	bool Passed() const { return passed;  }

	const std::string &GetError() const { return error; }

	NodePtr GetRoot() { return root; }

protected:
	std::vector<Token> tokens;
	std::vector<NodePtr> stack;
	int current;
	NodePtr root;
	bool passed;
	std::string error;
	int indent;

private:
	void Push(NodePtr node)
	{
		if (node)
			stack.push_back(node);
	}

	NodePtr Pop()
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

	bool PushConsume()
	{
		Push(NewNode(Consume()));
		return true;
	}

	Token const &Next()
	{
		return tokens[++current];
	}

	Token const &Last()
	{
		return tokens[current - 1];
	}

	Token const &Current() const
	{
		return tokens[current];
	}

	Token const &Peek() const
	{
		return tokens[current + 1];
	}

	bool PeekIs(TokenEnum ty) const
	{
		return Peek().type == ty;
	}
	Token const &Consume()
	{
		return tokens[current++];
	}

	bool Try(TokenEnum type)
	{
		return Current().type == type;
	}

	NodePtr Expect(TokenEnum type)
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

	void Print()
	{
		Print(*root, 0);
	}

	void Print(Node const &node, int level)
	{
		std::cout << Lead(level) << node << std::endl;

		for (auto ch : node.Children)
			Print(*ch, level + 1);
	}

	std::string ParserCommon::Lead(int level)
	{
		return std::move(std::string(level*4, ' '));
	}

	NodePtr NewNode(TokenEnum t) { return std::make_shared<Node>(t); }
	NodePtr NewNode(Token const &t) { return std::make_shared<Node>(t); }
};

KAI_END

