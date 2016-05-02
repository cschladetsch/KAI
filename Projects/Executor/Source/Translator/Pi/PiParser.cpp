#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/Structure.h"
#include "KAI/Translator/Pi/PiToken.h"
#include "KAI/Translator/Pi/PiParser.h"

#include <memory>
#include <iostream>
#include <algorithm>
#include <strstream>
#include <stdarg.h>

KAI_BEGIN

void PiParser::Process(Structure st)
{
	while (!Failed && NextSingle())
		root->Add(Pop());
}

bool PiParser::NextSingle()
{
	if (current == tokens.size() - 1)
		return false;

	auto tok = Peek();
	switch (tok.type)
	{
	case PiTokens::OpenSquareBracket:
		Consume();
		return ParseArray();

	case PiTokens::OpenBrace:
		Consume();
		return ParseContinuation();

	default:
		Push(NewNode(Consume()));
		return true;
	}
}

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
bool PiParser::ParseArray()
{
	auto node = NewNode(PiAstNodes::Array);
	while (!PeekIs(PiTokens::CloseSquareBracket))
	{
		if (NextSingle())
			node->Add(Pop());
		else
		{
			Fail("Malformed");
			return false;
		}
	}
	
	Consume();
	Push(node);

	return true;
}

bool PiParser::ParseContinuation()
{
	return false;
}


KAI_END