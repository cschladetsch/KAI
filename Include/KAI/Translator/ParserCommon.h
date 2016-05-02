#pragma once

#include "KAI/Translator/Structure.h"
#include "KAI/Translator/LexerCommon.h"
#include "KAI/Translator/AstNodeBase.h"
#include "KAI/Translator/ParserBase.h"

KAI_BEGIN

// common for all parsers.
// iterate over a stream of tokens to produce an abstract syntax tree
template <class Lexer, class AstEnumStruct>
struct ParserCommon : Process
{
	typedef Lexer Lexer;
	typedef typename Lexer::Token Token;
	typedef typename Token::Enum TokenEnum;
	typedef typename AstEnumStruct::Enum AstEnum;
	typedef AstNodeBase<Token, AstEnumStruct> AstNode;
	typedef std::shared_ptr<AstNode> AstNodePtr;

	bool Passed() const { return passed;  }
	const std::string &GetError() const { return error; }
	AstNodePtr GetRoot() { return root; }
	
	std::string Print()
	{
		std::stringstream str;
		Print(str, *root, 0);
		str << std::ends;
		return std::move(str.str());
	}

	ParserCommon(std::shared_ptr<Lexer> lex, Structure st = Structure::Statement)
	{
		current = 0;
		indent = 0;
		lexer = lex;

		if (lexer->Failed)
			return;

		// strip whitespace and comments
		for (auto tok : lexer->GetTokens())
			if (tok.type != TokenEnum::Whitespace && tok.type != TokenEnum::Comment)
				tokens.push_back(tok);

		root = NewNode(AstEnum::None);
	}

	virtual void Run(Structure st)
	{
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

protected:
	std::vector<Token> tokens;
	std::vector<AstNodePtr> stack;
	int current;
	AstNodePtr root;
	bool passed;
	std::string error;
	int indent;
	std::shared_ptr<Lexer> lexer;

protected:
	void Push(AstNodePtr node)
	{
		if (node)
			stack.push_back(node);
	}

	AstNodePtr Pop()
	{
		if (stack.empty())
		{
			//MUST CreateError("Internal Error: Parse stack empty");
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

	AstNodePtr Expect(TokenEnum type)
	{
		Token tok = Current();
		if (tok.type != type)
		{
			//MUST Fail(Lexer::CreateErrorMessage(tok, "Expected %s, have %s", Token::ToString(type), Token::ToString(tok.type)));
			KAI_THROW_1(LogicError, "Unexpected token");
		}

		Next();
		return std::make_shared<AstNode>(Last());
	}

	void Print(std::stringstream &out,  AstNode const &node, int level)
	{
		out << Lead(level) << node << std::endl;

		for (auto const &ch : node.Children)
			Print(out, *ch, level + 1);
	}

	std::string ParserCommon::Lead(int level)
	{
		return std::move(std::string(level*4, ' '));
	}

	AstNodePtr NewNode(AstEnum t) { return std::make_shared<AstNode>(t); }
	AstNodePtr NewNode(Token const &t) { return std::make_shared<AstNode>(t); }
};

KAI_END

