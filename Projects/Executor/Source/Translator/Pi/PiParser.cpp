#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/Structure.h"
#include "KAI/Translator/Pi/PiToken.h"
#include "KAI/Translator/Pi/PiParser.h"

#include <memory>
#include <iostream>
#include <algorithm>
#include <strstream>
#include <stdarg.h>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

KAI_BEGIN

void PiParser::Process(std::shared_ptr<Lexer> lex, Structure st)
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

	root = NewNode(AstEnum::Program);

	Run(st);
}

void PiParser::Run(Structure st)
{
	KAI_UNUSED_1(st);
	while (!Failed && NextSingle(root))
		;
}

bool PiParser::NextSingle(AstNodePtr root)
{
	if (current == tokens.size() - 1)
		return false;

	auto tok = Current();
	switch (tok.type)
	{
	case PiTokens::CloseSquareBracket:
	case PiTokens::CloseBrace:
		Fail(Lexer::CreateErrorMessage(Current(), "%s", "Unopened compound"));
		return false;

	case PiTokens::OpenSquareBracket:
		Consume();
		return ParseArray(root);

	case PiTokens::OpenBrace:
		Consume();
		return ParseContinuation(root);

	case PiTokens::Int:
		Top()->Add(std::make_shared<AstNode>(New(lexical_cast<int>(tok.Text()))));
		break;

	case PiTokens::Float:
		Top()->Add(std::make_shared<AstNode>(New(lexical_cast<float>(tok.Text()))));
		break;
	//case PiTokens::Plus:
	//	Top()->Add(std::make_shared<AstNode>(New(lexical_cast<float>(tok.Text()))));
	//	break;
	default:
		root->Add(NewNode(Consume()));
		return true;
	}
}

		//Appent

//bool PiParser::Compound()
//{
//	switch (Peek().type)
//	{
//	case PiTokens::OpenBrace:
//		return ParseContinuation();
//		break;
//
//	case PiTokens::OpenSquareBracket:
//		return ParseArray();
//		break;
//	}
//
//	return false;
//}
//
bool PiParser::ParseArray(AstNodePtr root)
{
	auto node = NewNode(PiAstNodes::Array);
	while (!Failed && Current().type != PiTokens::CloseSquareBracket)
	{
		if (!NextSingle(node))
		{
			Fail("Malformed Array");
			return false;
		}
	}
	Consume();

	root->Add(node);
	
	return true;
}

bool PiParser::ParseContinuation(AstNodePtr root)
{
	return false;
}


KAI_END