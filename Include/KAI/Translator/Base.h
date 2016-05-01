#pragma once

#include "KAI/Translator/Config.h"

KAI_BEGIN

template <class AstEnumStruct>
struct TokenBase;

template <class Token>
struct LexerCommon;

template <class AstEnum>
struct NodeBase;

template <class Lexer, class AstEnumStruct>
struct ParserCommon;

KAI_END
