#pragma once

KAI_BEGIN

class PiTranslator;
class RhoTranslator;
class TauTranslator;

enum class Language { None, Pi, Rho, Tau };

//template <enum class Language>
//struct TranslationProvider { typedef PiTranslator Lang; };
//
//template <>
//struct TranslationProvider<Language::Rho> { typedef RhoTranslator Lang; };
//
//template <>
//struct TranslationProvider<Language::Tau> { typedef void Lang; };

KAI_END

