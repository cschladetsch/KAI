#pragma once

#include <KAI/Language/Common/ParserCommon.h>
#include <KAI/Language/Rho/RhoAstNode.h>
#include <KAI/Language/Rho/RhoLexer.h>

KAI_BEGIN

// parser specific to the in-fix Rho language
class RhoParser : public ParserCommon<RhoLexer, RhoAstNodeEnumType>
{
public:
    typedef ParserCommon<RhoLexer, RhoAstNodeEnumType> Parent;
    using typename Parent::TokenEnum;
    using typename Parent::TokenNode;
    using typename Parent::Lexer;
    using typename Parent::AstNode;
    using typename Parent::AstNodePtr;

    typedef RhoAstNodeEnumType NodeType; 
    typedef RhoTokenEnumType TokenType;

    RhoParser(Registry &r) : Parent(r) { }

    virtual bool Process(std::shared_ptr<Lexer> lex, Structure st) override;
    String Print() const;

protected:
    void Process(Structure);

private:
    bool Run(Structure st);
    bool Program();
    bool Statement(AstNodePtr);
    bool Expression();
    bool Logical();
    bool Relational();
    bool Additive();
    bool Term();
    bool Factor();
    bool ConsumeNewLines();
    bool Block(AstNodePtr block);
    bool ParsePathname();
    bool ParsePathname(AstNodePtr path);
    bool ParseFactorIdent();
    bool ParseGetMember();
    bool ParseMethodCall();
    bool Function(AstNodePtr);
    bool AddBlock(AstNodePtr fun);
    bool IfCondition(AstNodePtr);
    bool ParseIndexOp();
    bool Assignment(AstNodePtr);
    bool For(AstNodePtr);
    bool While(AstNodePtr);

    bool CreateError(const char *);
};

KAI_END
