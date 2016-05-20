#pragma once

#include <KAI/Language/Common/LangCommon.h>
#include "KAI/Language/Tau/TauTranslator.h"

KAI_BEGIN

/// Tau is an interface lagnauge used to describe an Agent in the system
/// It is wht is commonly called an 'IDL'
class TauLang : Lang<TauTranslator>
{
	typedef Lang<TauTranslator> Parent;

	TauLang(Registry &r) : Parent(r) { }

	Pointer<Executor> Exec(const char *text, Structure st = Structure::Expression);
	Pointer<Continuation> Translate(const char *text, Structure st = Structure::Expression);
	Pointer<Continuation> TranslateFile(const char *name, Structure st = Structure::Program);

	void Print();
};

KAI_END
