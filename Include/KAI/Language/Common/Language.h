#pragma once

KAI_BEGIN

class PiTranslator;
class RhoTranslator;

enum class Language { None, Pi, Rho, Tau, Hlsl };
const char *ToString(Language);

KAI_END
