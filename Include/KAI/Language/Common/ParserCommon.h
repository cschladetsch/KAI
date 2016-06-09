#pragma once

#include <boost/lexical_cast.hpp>
#include "KAI/Language/Common/ParserBase.h"
#include "KAI/Language/Common/ProcessCommon.h"
#include "KAI/Language/Common/Process.h"
#include "KAI/Language/Common/AstNodeBase.h"
#include "KAI/Language/Common/Structure.h"
#include <iostream>

using namespace std;

KAI_BEGIN

//using lexical_cast = boost::lexical_cast;

// common for all parsers.
// iterate over a stream of tokens to produce an abstract syntax tree
template <class ELexer, class AstEnumStruct>
class ParserCommon : public ProcessCommon
{
public:
	typedef ELexer Lexer;
	typedef typename Lexer::Token TokenNode;
	typedef typename Lexer::TokenEnumType TokenEnumType;
	typedef typename TokenNode::Enum TokenEnum;
	typedef typename AstEnumStruct::Enum AstEnum;
	typedef AstNodeBase<TokenNode, AstEnumStruct> AstNode;
	typedef std::shared_ptr<AstNode> AstNodePtr;

	const std::string &GetError() const { return error; }
	AstNodePtr GetRoot() const { return root; }
	
	ParserCommon(Registry& r) : ProcessCommon(r)
	{ 
		current = 0;
		indent = 0;
		lexer.reset();
	}

	virtual void Process(std::shared_ptr<Lexer> lex, Structure st) = 0;

//	template <class T>
//	AstNodePtr AppendLexicalValue(TokenNode const &tok)
//	{
//		Pointer<T> val = reg.New(boost::lexical_cast<T>(tok.Text()));
//		return std::make_shared<
//	}

	template <class T>
	Pointer<T> New(T const &val)
	{
		return _reg.New<T>(val);
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

	std::string PrintTree() const
	{
		std::stringstream str;
		PrintTree(str, 0, root);
		return std::move(str.str());
	}

	std::string ToString() const
	{
		return std::move(root->ToString());
	}

protected:
	void PrintTree(std::ostream &str, int level, AstNodePtr root) const
	{
		std::string indent(4*level, ' ');
		str << indent << root->ToString().c_str() << std::endl;
		for (auto const &ch : root->GetChildren())
		{
			PrintTree(str, level + 1, ch);
		}
	}

	std::vector<TokenNode> tokens;
	std::vector<AstNodePtr> stack;
	size_t current;
	AstNodePtr root;
	std::string error;
	int indent;
	std::shared_ptr<Lexer> lexer;

protected:
	bool Has() const
	{
		return current < tokens.size();
	}

	void Push(AstNodePtr node)
	{
		if (node)
			stack.push_back(node);
	}

	void Append(Object Q)
	{
		Top()->Children.push_back(std::make_shared<AstNode>(AstEnum::Object, Q));
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
		if (!Has())
		{
			Fail(Lexer::CreateErrorMessage(TokenNode(), "Expected something"));
			KAI_THROW_1(LogicError, "Expected something");
			//return TokenNode::None;
		}

		return tokens[current];
	}

	bool Current(TokenNode node) const
	{
		if (current >= tokens.size())
		{
			return false;
		}

		return tokens[current] == node;
	}

	bool Empty() const
	{
		return current >= tokens.size();
	}

	TokenNode const &Peek() const
	{
		return tokens[current + 1];
	}

	bool PeekConsume(TokenEnum ty)
	{
		if (tokens[current + 1].type == ty)
		{
			Consume();
			return true;
		}

		return false;
	}

	bool CurrentIs(TokenEnum ty) const
	{
		return Current().type == ty;
	}

	bool PeekIs(TokenEnum ty) const
	{
		return Peek().type == ty;
	}

	bool Consume(TokenEnum ty)
	{
		if (Current().type == ty)
		{
			Consume();
			return true;
		}

		return false;
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
			//Fail(Lexer::CreateErrorMessage(tok, "Expected %s, have %s", TokenEnum::ToString(type), TokenEnum::ToString(tok.type)));
			Fail(Lexer::CreateErrorMessage(tok, "Unexpected token %s", TokenEnumType::ToString(tok.type)));
			cerr << Error << endl;
			KAI_THROW_1(LogicError, "Unexpected token");
		}

		Next();
		return std::make_shared<AstNode>(Last());
	}

	AstNodePtr NewNode(AstEnum t) { return std::make_shared<AstNode>(t); }
	AstNodePtr NewNode(AstEnum e, TokenNode const &t) const { return std::make_shared<AstNode>(e,t); }
	AstNodePtr NewNode(TokenNode const &t) { return std::make_shared<AstNode>(t); }
};

KAI_END

