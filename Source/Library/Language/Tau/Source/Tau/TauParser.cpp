#include <KAI/Language/Common/ParserCommon.h>
#include <KAI/Language/Tau/TauParser.h>
#include <assert.h>

using namespace std;

TAU_BEGIN

bool TauParser::Process(shared_ptr<Lexer> lex, Structure st)
{
    // Tau always starts at Module level (a series of namespaces)
    assert(st == Structure::Module);
    KAI_UNUSED_1(st);

    current = 0;
    indent = 0;
    lexer = lex;

    if (lexer->Failed)
        return Fail("Lexer error: " + lexer->Error);

    // strip whitespace and comments
    for (auto const &tok : lexer->GetTokens())
    {
        switch (tok.type)
        {
        case TokenEnum::Whitespace:
        case TokenEnum::Tab:
        case TokenEnum::Comment:
        case TokenEnum::NewLine:
            continue;
        }

        tokens.push_back(tok);
    }

    root = NewNode(AstEnum::Module);

    return Run(Structure::Module);
}

bool TauParser::Run(Structure st)
{
    KAI_UNUSED_1(st);
    if (Empty())
    {
        KAI_TRACE_WARN_1("Nothing to parse");
        return true;
    }

    while (!Empty() && Current().type == TokenType::Namespace)
    {
        Consume();
        Namespace(root);
        if (Failed)
            return false;
    }

    return true;
}

void TauParser::Namespace(AstNodePtr root)
{
    auto ns = NewNode(TauAstEnumType::Namespace, Consume());
    Expect(TokenEnum::OpenBrace);

    while (!Empty() && !CurrentIs(TokenEnum::CloseBrace))
    {
        switch (Current().type)
        {
            case TokenEnum::Class:
                Consume();
                Class(ns);
                break;

            case TokenEnum::Namespace:
                Consume();
                Namespace(ns);
                break;

            default:
            {
                auto const &cur = Current();
                Fail(Lexer::CreateErrorMessage(cur, "Unexpected token %s", TokenEnumType::ToString(cur.type)));
            }
        }

        if (Failed)
            return;
    }

    Expect(TokenEnumType::CloseBrace);
    OptionalSemi();
    root->Add(ns);
}

void TauParser::Class(AstNodePtr root)
{
    auto klass = NewNode(TauAstEnumType::Class, Consume());
    Expect(TokenEnum::OpenBrace);

    while (!Failed && !Empty() && !CurrentIs(TokenEnum::CloseBrace))
    {
        // Expect a series of methods and properties.
        // Either way, start with a type name and identifier.
        auto ty = Expect(TokenEnum::Ident)->GetToken();
        auto name = Expect(TokenEnum::Ident)->GetToken();

        if (CurrentIs(TokenType::OpenParan))
        {
            Consume();
            Method(klass, ty, name);
        }
        else
        {
            Field(klass, ty, name);
        }
    }

    Expect(TokenEnum::CloseBrace);
    OptionalSemi();
    root->Add(klass);
}

void TauParser::Method(AstNodePtr klass, TokenNode const &returnType, TokenNode const &name)
{
    auto method = NewNode(AstEnum::Method, name);
    auto args = NewNode(AstEnum::Arglist);

    method->Add(returnType);
    method->Add(args);

    while (!CurrentIs(TokenType::CloseParan))
    {
        AddArg(args);
        if (!CurrentIs(TokenType::Comma))
            break;

        Consume();
    }

    Expect(TokenType::CloseParan);
    OptionalSemi();
    klass->Add(method);
}

void TauParser::Field(AstNodePtr klass, TokenNode const &ty, TokenNode const &id)
{
    auto field = NewNode(AstEnum::Property);
    field->Add(ty);
    field->Add(id);
    OptionalSemi();
    klass->Add(field);
}

void TauParser::OptionalSemi()
{
    if (CurrentIs(TokenType::Semi) || PeekIs(TokenType::Semi))
        Consume();
}

void TauParser::AddArg(AstNodePtr parent)
{
    auto arg = NewNode(AstEnum::Argument);
    arg->Add(Consume());    // type
    arg->Add(Consume());    // name
    parent->Add(arg);
}

TAU_END

