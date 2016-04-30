#pragma once

#include "KAI/Translator/Config.h"

KAI_BEGIN

template <class Token>
struct LexerCommon;

template <class AstEnum>
struct NodeBase;

template <class Lexer, class Node>
struct ParserCommon;

KAI_END
