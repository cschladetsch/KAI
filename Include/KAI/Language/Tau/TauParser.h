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

    virtual bool Process(std::shared_ptr<Lexer> lex, Structure st) override;

protected:
    bool Run(Structure);

    void Namespace(AstNodePtr rent);
    void Class(AstNodePtr rent);
    void Method(AstNodePtr klass, TokenNode const &returnType, TokenNode const &name);
    void Field(AstNodePtr klass, TokenNode const &ty, TokenNode const &id);
    //void Event(AstNodePtr klass, TokenNode const &ty, TokenNode const &id);

    void AddArg(AstNodePtr parent);
    void OptionalSemi();
};

TAU_END
