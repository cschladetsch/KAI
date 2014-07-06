#pragma once

#include <string>
#include <strstream>

#include "Parser.h"

//#define SCHLADETSCH_NAMESPACE KAI_NAMESPACE_NAME
//#include <EventP.h>

KAI_TRANS_BEGIN

struct Translator : Process
{
	//KAI_NAMESPACE_NAME::Events::Event<Translator, Token> 
	std::strstream result;

	Translator(Parser const *p);

	std::string Result() { return result.str(); }

private:
	typedef Parser::NodePtr NodePtr;

	void Traverse(NodePtr node);
	void TranslateFunction(NodePtr node);
	void TranslateBlock(NodePtr node);
	void Translate(NodePtr node);
	void TranslateBinaryOp(NodePtr node, std::string op);

	// TODO: not create strings on each call: store the conversion in a map or something
	std::string ConvertOp(NodePtr node);

	void TranslateFromToken(NodePtr node);
	void TranslateCall(NodePtr node);
	void TranslateIndex(NodePtr node);
	void BinaryOp(NodePtr node, const char *op);
};

KAI_TRANS_END
