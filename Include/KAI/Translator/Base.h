#pragma once

#include "KAI/Translator/Config.h"

KAI_BEGIN

template <class AstEnumStruct>
class TokenBase;

template <class Token>
class LexerCommon;

template <class AstEnum>
class NodeBase;

template <class Lexer, class AstEnumStruct>
class ParserCommon;

KAI_END
