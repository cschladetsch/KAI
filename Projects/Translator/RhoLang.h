#pragma once

#include "Base.h"
#include "Process.h"
#include "Parser.h"

KAI_TRANS_BEGIN

struct RhoLang : Process
{
	Lexer *lex;
	Parser *parse;
	Translator *trans;

	void Print();

	bool Translate(const char *text, Parser::Structure st = Parser::ParseProgram);
	bool TranslateFile(const char *name, Parser::Structure st = Parser::ParseProgram);
};

KAI_TRANS_END
