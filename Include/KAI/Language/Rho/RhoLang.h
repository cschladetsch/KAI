#pragma once

#include "KAI/Language/Rho/RhoTranslator.h"

KAI_BEGIN

class RhoLang : Lang<RhoTranslator>
{
	typedef Lang<RhoTranslator> Parent;

	RhoLang(Registry &r) : Parent(r) { }

	Pointer<Executor> Exec(const char *text, Structure st = Structure::Expression);
	Pointer<Continuation> Translate(const char *text, Structure st = Structure::Expression);
	Pointer<Continuation> TranslateFile(const char *name, Structure st = Structure::Program);

	void Print();
};

KAI_END
