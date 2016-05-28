#pragma once

#include <KAI/Language/Common/LangCommon.h>
#include "Tau/Config.h"
#include "Tau/TauTranslator.h"

TAU_BEGIN

/// Tau is an interface language used to describe an Agent in the system
/// It is wht is commonly called an 'IDL'
class TauLang : Lang<TauTranslator>
{
	typedef Lang<TauTranslator> Parent;

	TauLang(Registry &r) : Parent(r) { }

	Pointer<Continuation> Translate(const char *text, Structure st = Structure::Expression);
	Pointer<Continuation> TranslateFile(const char *name, Structure st = Structure::Program);

	void Print();
};

TAU_END
