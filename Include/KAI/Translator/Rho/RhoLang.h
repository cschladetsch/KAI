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
	typedef typename RhoTranslator::Parser Parser;
	typedef typename Parser::Lexer Lexer;
	typedef typename RhoStructure Structure;

	std::shared_ptr<Lexer> lex;
	std::shared_ptr<Parser> parse;
	std::shared_ptr<RhoTranslator> trans;

	RhoLang(const RhoLang&) = delete;
	RhoLang(Registry &);

	Registry &reg;
	Pointer<Executor> exec;

	void Print();

	void Exec(const char *text, RhoStructure st = RhoStructure::ParseExpression);

	Pointer<Continuation> ProcessFile(const char *fileName);

	bool Translate(const char *text, RhoStructure st = RhoStructure::ParseProgram);
	bool TranslateFile(const char *name, RhoStructure st = RhoStructure::ParseProgram);
};

KAI_END
