#pragma once

#include "KAI/Translator/Base.h"
#include "KAI/Translator/Process.h"
#include "KAI/Translator/Parser.h"
#include "KAI/Translator/Translator.h"
#include "KAI/Executor.h"

KAI_BEGIN

struct RhoLang : Process//, boost::noncopyable
{
	RhoLang(const RhoLang&) = delete;
	RhoLang(Registry &);

	std::shared_ptr<Lexer> lex;
	std::shared_ptr<Parser> parse;
	std::shared_ptr<Translator> trans;

	Registry &reg;
	Pointer<Executor> exec;


	void Print();

	void Exec(const char *text, Parser::Structure st = Parser::ParseExpression);

	Pointer<Continuation> ProcessFile(const char *fileName);

	bool Translate(const char *text, Parser::Structure st = Parser::ParseProgram);
	bool TranslateFile(const char *name, Parser::Structure st = Parser::ParseProgram);
};

KAI_END
