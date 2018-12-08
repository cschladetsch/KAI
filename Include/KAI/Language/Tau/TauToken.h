#pragma once

#include <KAI/Language/Tau/Config.h>

TAU_BEGIN

struct TauTokenEnumType
{
    enum Enum
    {
        None = 0,
        Ident = 1,
        OpenParan = 2,
        CloseParan = 3,
        Value = 4,
        Const = 5,
        Reference = 9,
        Proxy = 11,
        Agent = 12,
        Semi = 13,
        NewLine = 14,
        OpenBrace = 15,
        CloseBrace = 16,
        Module = 17,
        Namespace = 18,
        ConstProxy = 19,
        ConstAgent = 20,
        ArrayProxy = 27,
        QuotedIdent = 28,
        Array = 29,
        Sync = 31,
        Class = 32,
        Comma = 33,
        Whitespace  = 34,
        Comment = 35,
        Async = 36,
        Tab = 37,
        String = 38,
    };

    struct Type : TokenBase<TauTokenEnumType>
    {
        typedef TokenBase<TauTokenEnumType> Parent;

        Type() { }
        Type(Enum val, const LexerBase &lexer, int ln, Slice slice)
                : Parent(val, lexer, ln, slice) { }
    };

    static const char *ToString(Enum val);
};

typedef TauTokenEnumType::Type TauToken;
typedef TauTokenEnumType::Enum TauTokenEnum;

TAU_END

