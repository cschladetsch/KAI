#pragma once

#include <memory>

#include "KAI/Translator/Lexer.h"
#include "KAI/Translator/Node.h"

KAI_BEGIN

// iterate over a stream of tokens to produce an abstract syntax tree
struct Parser : Process
{
	typedef std::shared_ptr<Node> NodePtr;

	// what type of thing to parse
	enum Structure
	{
		ParseStatement, ParseExpression, ParseFunction, ParseProgram,
	};

	Parser(std::shared_ptr<Lexer> lexer, Structure st = ParseProgram);

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

	bool PushConsume();

	Token const &Next();
	Token const &Last();
	Token const &Current() const;
	Token const &Peek() const;
	bool PeekIs(Token::Type ty) const;
	Token const &Consume();
	int indent;

	bool Try(Token::Type type);
	NodePtr Expect(Token::Type type);

	std::string Lead(int level);
	void Print(Node const &node, int level);
	void Block(NodePtr block);
	bool ParseFactorIdent();

	void ParseGetMember();

	void ParseMethodCall();
	void Function(NodePtr);

	void AddBlock(NodePtr fun);

	NodePtr NewNode(Node::Type t);
	NodePtr NewNode(Token const &);
	void IfCondition(NodePtr block);
	void ParseIndexOp();
	void Assignment(NodePtr);
	void For(NodePtr block);
	void While(NodePtr block);
	bool CreateError(const char *text);
	void ConsumeNewLines();
};

KAI_END

