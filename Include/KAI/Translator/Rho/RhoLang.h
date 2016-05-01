#pragma once

#include "KAI/Translator/Base.h"
#include "KAI/Translator/Process.h"
#include "KAI/Translator/ParserCommon.h"

#include "KAI/Translator/Rho/RhoToken.h"
#include "KAI/Translator/Rho/RhoLexer.h"
#include "KAI/Translator/Rho/RhoParser.h"
#include "KAI/Translator/Rho/RhoTranslator.h"

#include "KAI/Executor.h"

KAI_BEGIN

struct RhoLang : Process
{
	typedef RhoLexer Lexer;
	typedef RhoParser Parser;
	typedef RhoTranslator Translator;

	std::shared_ptr<Lexer> lex;
	std::shared_ptr<Parser> parse;
	std::shared_ptr<RhoTranslator> trans;

	RhoLang(const RhoLang&) = delete;
	RhoLang(Registry &);

	Registry &reg;
	Pointer<Executor> exec;

	void Print();

	void Exec(const char *text, Structure st = Structure::Expression);

	Pointer<Continuation> ProcessFile(const char *fileName);

	bool Translate(const char *text, Structure st = Structure::Program);
	bool TranslateFile(const char *name, Structure st = Structure::Program);
};

KAI_END
