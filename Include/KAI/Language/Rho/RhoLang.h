#pragma once

#include "KAI/Language/Common/Base.h"
#include "KAI/Language/Common/Process.h"
#include "KAI/Language/Common/ParserCommon.h"

#include "KAI/Language/Rho/RhoToken.h"
#include "KAI/Language/Rho/RhoLexer.h"
#include "KAI/Language/Rho/RhoParser.h"
#include "KAI/Language/Rho/RhoTranslator.h"

#include "KAI/Executor/Executor.h"

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
