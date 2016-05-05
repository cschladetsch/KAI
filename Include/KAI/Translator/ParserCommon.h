#pragma once

#include "KAI/Translator/Structure.h"
#include "KAI/Translator/LexerCommon.h"
#include "KAI/Translator/AstNodeBase.h"
#include "KAI/Translator/ParserBase.h"

KAI_BEGIN

// common for all parsers.
// iterate over a stream of tokens to produce an abstract syntax tree
template <class Lexer, class AstEnumStruct>
struct ParserCommon : CommonBase
{
	typedef Lexer Lexer;
	typedef typename Lexer::Token TokenNode;
	typedef typename TokenNode::Enum TokenEnum;
	typedef typename AstEnumStruct::Enum AstEnum;
	typedef AstNodeBase<TokenNode, AstEnumStruct> AstNode;
	typedef std::shared_ptr<AstNode> AstNodePtr;

	bool Passed() const { return passed;  }
	const std::string &GetError() const { return error; }
	AstNodePtr GetRoot() { return root; }
	
	ParserCommon(Registry& r) : CommonBase(r)
	{ 
		current = 0;
		indent = 0;
		lexer.reset();
	}

	std::string Print() const
	{
		std::stringstream str;
		Print(str, 0, root);
		return str.str();
	}

	void Print(std::stringstream &str, int level, AstNodePtr root)
	{
		str << std::string(' ', 4*level) << root << std::ends;
		for (auto char &ch : root->Children)
			print(str, level + 1, ch);
	}

	virtual void Process(std::shared_ptr<Lexer> lex, Structure st) = 0;

	template <class T>
	AstNodePtr AppendLexicalValue(TokenNode const &tok)
	{
		return Append(registry.New(std::lexical_cast<T>(tok.Text())));
	}

	template <class T>
	Pointer<T> New(T const &val)
	{
		return reg.New<T>(val);
	}

	void Run(Structure st)
	{
		try
		{
			Process(st);
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

	std::string Print()
	{
		std::stringstream str;
		Print(str, *root, 0);
		str << std::endl << std::ends;
		return str.str();
	}

protected:
	std::vector<TokenNode> tokens;
	std::vector<AstNodePtr> stack;
	size_t current;
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

	void Append(Object Q)
	{
		Top()->Children.push_back(make_shared<AstNode>(AstEnum::Object, Q));
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

	AstNodePtr Top()
	{
		return stack.back();
	}

	bool PushConsume()
	{
		Push(NewNode(Consume()));
		return true;
	}

	TokenNode const &Next()
	{
		return tokens[++current];
	}

	TokenNode const &Last()
	{
		return tokens[current - 1];
	}

	TokenNode const &Current() const
	{
		return tokens[current];
	}

	TokenNode const &Peek() const
	{
		return tokens[current + 1];
	}

	void PeekConsume(TokenEnum ty)
	{
		if (tokens[current + 1] == ty)
			Consume();
	}

	bool PeekIs(TokenEnum ty) const
	{
		return Peek().type == ty;
	}

	TokenNode const &Consume()
	{
		return tokens[current++];
	}

	bool Try(TokenEnum type)
	{
		return Current().type == type;
	}

	AstNodePtr Expect(TokenEnum type)
	{
		TokenNode tok = Current();
		if (tok.type != type)
		{
			//MUST Fail(Lexer::CreateErrorMessage(tok, "Expected %s, have %s", Token::ToString(type), Token::ToString(tok.type)));
			KAI_THROW_1(LogicError, "Unexpected token");
		}

		Next();
		return std::make_shared<AstNode>(Last());
	}

	void Print(std::ostream &out, AstNode const &node, int level)
	{
		out << Lead(level) << node << std::endl;
		//std::cout << Lead(level) << node << std::endl;

		for (auto const &ch : node.Children)
			Print(out, *ch, level + 1);
	}

	std::string ParserCommon::Lead(int level)
	{
		return std::move(std::string(level*4, ' '));
	}

	AstNodePtr NewNode(AstEnum t) { return std::make_shared<AstNode>(t); }
	AstNodePtr NewNode(TokenNode const &t) { return std::make_shared<AstNode>(t); }
};

KAI_END

