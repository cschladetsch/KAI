#pragma once

#include <KAI/Language/Common/ParserCommon.h>
#include <KAI/Language/Tau/TauLexer.h>
#include <KAI/Language/Tau/TauAstNode.h>

TAU_BEGIN

// parser specific to the IDL Tau language
class TauParser : public ParserCommon<TauLexer, TauAstEnumType>
{
public:
    typedef ParserCommon<TauLexer, TauAstEnumType> Parent;
    using Parent::TokenEnum;
    using Parent::TokenNode;
    using Parent::Lexer;
    using Parent::AstNode;
    using Parent::AstNodePtr;

    typedef TauAstEnumType::Enum AstType;
    typedef TauAstEnumType NodeType;
    typedef TauTokenEnumType TokenType;

    TauParser(Registry &r) : Parent(r) { }

    void StripTokens();
    bool Process(std::shared_ptr<Lexer> lex, Structure st) override;

protected:
    bool Run(AstNodePtr, Structure);
    bool Namespace(AstNodePtr root);
    bool Module(AstNodePtr root);
    bool Class(AstNodePtr root);
    bool Method(AstNodePtr klass, TokenNode const &returnType, TokenNode const &name);
    bool Field(AstNodePtr klass, TokenNode const &ty, TokenNode const &id);
    bool Event(AstNodePtr klass, TokenNode const &ty, TokenNode const &id);

    void AddArg(AstNodePtr list);
    //void OptionalSemi();
};

TAU_END

//EOF
