#pragma once

KAI_BEGIN

template <class Lexer, class Node>
struct ParserBase : Process
{
	typedef Lexer Lexer;
	typedef Node Node;
	typedef typename Lexer::Token Token;
	typedef typename Token::Enum TokenEnum;
	typedef std::shared_ptr<Node> NodePtr;
};

KAI_END

