#pragma once

#include <KAI/Language/Common/ParserCommon.h>
#include <KAI/Language/Common/Structure.h>
#include <KAI/Language/Pi/PiAstNode.h>
#include <KAI/Language/Pi/PiLexer.h>

KAI_BEGIN

// parser specific to the post-fix Pi language
struct PiParser : ParserCommon<PiLexer, PiAstNodeEnumType>
{
    typedef ParserCommon<PiLexer, PiAstNodeEnumType> Parent;
    using Parent::Lexer;
    using Parent::AstNode;
    using Parent::TokenNode;
    using Parent::TokenEnum;
    using Parent::AstNodePtr;
    typedef PiAstNodeEnumType NodeType;
    typedef PiTokenEnumType TokenType;

    PiParser(Registry &r) : Parent(r) { }

    bool Process(std::shared_ptr<Lexer> lex, Structure st) override;

private:
    bool Run(Structure);
    bool NextSingle(AstNodePtr context);
    bool ParseArray(AstNodePtr context);
    bool ParseContinuation(AstNodePtr context);
    bool ParseCompound(AstNodePtr root, AstEnum nodeType, TokenEnum end);
};

KAI_END

