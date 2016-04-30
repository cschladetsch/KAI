#pragma once

#include "KAI/Translator/TokenBase.h"
#include "KAI/Translator/AstNodeBase.h"
#include "KAI/Translator/ParserCommon.h"

KAI_BEGIN

// parser specific to the in-fix Rho language
template <class Lexer, class Node>
struct RhoParser : ParserCommon<Lexer, Node>
{
	typedef ParserCommon<Lexer, Node> Parent;
	using typename Parent::Lexer;
	using typename Parent::Node;
	using typename Parent::Token;
	using typename Parent::TokenType;
	using typename Parent::NodeType;
	using typename Parent::NodePtr;

	RhoParser(std::shared_ptr<Lexer> lexer, RhoStructure st = RhoStructure.ParseProgram);

	void Run(RhoStructure st);
	void ConsumeNewLines();
	void Block(NodePtr block);
	bool ParseFactorIdent();
	void ParseGetMember();
	void ParseMethodCall();
	void Function(NodePtr);
	void AddBlock(NodePtr fun);
	bool Program();
	bool Statement(NodePtr );
	bool Expression();
	bool Logical();
	bool Relational();
	bool Additive();
	bool Term();
	bool Factor();
	void IfCondition(NodePtr block);
	void ParseIndexOp();
	void Assignment(NodePtr);
	void For(NodePtr block);
	void While(NodePtr block);
	bool CreateError(const char *text);
};

KAI_END

