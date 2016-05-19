#include "KAI/ExecutorPCH.h"

#include <iostream>
#include <strstream>

using namespace std;
using namespace boost;

KAI_BEGIN

void PiParser::Process(std::shared_ptr<Lexer> lex, Structure st)
{
	current = 0;
	indent = 0;
	lexer = lex;

	if (lexer->Failed)
	{
		Failed = true;
		return;
	}

	// strip whitespace and comments
	for (auto tok : lexer->GetTokens())
		if (tok.type != TokenEnum::Whitespace && tok.type != TokenEnum::Comment)
			tokens.push_back(tok);

	root = NewNode(AstEnum::Continuation);

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
	//case PiTokens::CloseSquareBracket:
	//case PiTokens::CloseBrace:
	//	Fail(Lexer::CreateErrorMessage(Current(), "%s", "Unopened compound"));
	//	return false;

	case PiTokens::OpenSquareBracket:
		Consume();
		return ParseArray(root);

	case PiTokens::OpenBrace:
		Consume();
		return ParseContinuation(root);

	case PiTokens::Clear:
	case PiTokens::Drop:
	case PiTokens::Dup:
	case PiTokens::Rot:
	case PiTokens::PickN:
	case PiTokens::Pathname:

	case PiTokens::String:
	case PiTokens::Int:
	case PiTokens::Float:
	case PiTokens::True:
	case PiTokens::False:

	case PiTokens::Plus:
	case PiTokens::Minus:
	case PiTokens::Mul:
	case PiTokens::Divide:

	case PiTokens::PlusAssign:
	case PiTokens::MinusAssign:
	case PiTokens::MulAssign:
	case PiTokens::DivAssign:
	default:
		root->Add(Consume());
		return true;
	}

	return false;
}

bool PiParser::ParseArray(AstNodePtr root)
{
	auto node = NewNode(PiAstNodes::Array);
	while (!Failed && !PeekIs(PiTokens::CloseSquareBracket))
	{
		if (!NextSingle(node))
		{
			Fail("Malformed Array");
			return false;
		}
	}

	if (Failed)
		return false;

	Consume();
	root->Add(node);

	return true;
}

bool PiParser::ParseContinuation(AstNodePtr root)
{
	AstNodePtr node = NewNode(PiAstNodes::Continuation);
	while (!Failed && !Try(PiTokenEnumType::CloseBrace))
	{
		if (!NextSingle(node))
		{
			Fail("Malformed Coro");
			return false;
		}
	}

	if (Failed)
		return false;
	Consume();

	root->Add(node);

	return true;
}


KAI_END