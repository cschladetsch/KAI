#pragma once

#include <KAI/Language/Common/LangCommon.h>
#include "Tau/TauTranslator.h"

KAI_BEGIN

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

KAI_END
