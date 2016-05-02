#pragma once

#include "KAI/Translator/Pi/PiParser.h"
#include "KAI/Executor.h"

KAI_BEGIN

struct PiLang : Process
{
	typedef PiLexer Lexer;
	typedef PiParser Parser;

	std::shared_ptr<Lexer> lex;
	std::shared_ptr<Parser> parse;


	PiLang(const PiLang&) = delete;
	PiLang(Registry &);

	Registry &reg;
	Pointer<Executor> exec;

	void Print();

	void Exec(const char *text, Structure st = Structure::Expression);

	Pointer<Continuation> ProcessFile(const char *fileName);

	bool Translate(const char *text, Structure st = Structure::Program);
	bool TranslateFile(const char *name, Structure st = Structure::Program);
};

KAI_END
