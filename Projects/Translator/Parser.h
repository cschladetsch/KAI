#pragma once

#include <memory>

#include "Lexer.h"
#include "Node.h"

KAI_TRANS_BEGIN

// iterate over a stream of tokens to produce an abstract syntax tree
struct Parser : Process
{
	struct Exception { };
	struct StackError : Exception { };
	struct Unexpected : Exception
	{
		Unexpected(Token::Type ty);
	};

	typedef std::shared_ptr<Node> NodePtr;

	// what type of thing to parse
	enum Structure
	{
		ParseStatement, ParseExpression, ParseFunction, ParseProgram,
	};

	Parser(Lexer const *lexer, Structure st = ParseProgram);

	void Print();

	bool Passed() const { return passed;  }
	const std::string &GetError() const { return error; }

private:
	friend struct Translator;
	std::vector<Token> tokens;
	std::vector<NodePtr> stack;
	int current;
	NodePtr root;
	bool passed;
	std::string error;

private:
	void Run(Structure st);
	void Push(NodePtr node);
	NodePtr Pop();

	bool Program();
	bool Statement(NodePtr );
	bool Expression();

	bool Logical();
	bool Relational();
	bool Additive();
	bool Term();
	bool Factor();

	void PushConsume();
	//bool Error(const char *fmt, ...);
	//bool Error(const char *fmt, Token tok);
	//void Fail(const char *fmt);

	Token const &Next();
	Token const &Last();
	Token const &Current() const;
	Token const &Peek() const;
	bool PeekIs(Token::Type ty) const;
	Token const &Consume();

	bool Try(Token::Type type);
	NodePtr Expect(Token::Type type);

	std::string Lead(int level);
	void Print(Node const &node, int level);
	void Block(NodePtr block);
	bool ParseFactorIdent();

	void ParseGetMember();

	void ParseMethodCall();
	void Function(NodePtr);
	Parser::NodePtr NewNode(Node::Type t);
	Parser::NodePtr NewNode(Token const &);
	void IfCondition(NodePtr block);
	void ParseIndexOp();
};

KAI_TRANS_END

