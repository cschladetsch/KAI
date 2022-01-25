#include "KAI/Language/Lisp/LispToken.h"

KAI_BEGIN

#undef CASE
#undef CASE_LOWER
#undef CASE_REPLACE

const char *LispTokenEnumType::ToString(Enum val)
{
    switch (val)
    {
        #define CASE(N) case LispTokens::N : return #N;
        #define CASE_LOWER(N) case LispTokens::N : return ToLower(#N);
        #define CASE_REPLACE(N, M) case LispTokens::N : return M;

        CASE(None)
        CASE(Whitespace)
        CASE(Semi)
        CASE(Int)
        CASE(Float )
        CASE(String )
        CASE(True )
        CASE(False)
        CASE(Return)
        CASE(Ident)
        CASE(Dot)
        CASE(Comma)
        CASE(If )
        CASE(Else)
        CASE(For )
        CASE(While)
        CASE(OpenBrace )
        CASE(CloseBrace)
        CASE(OpenParan )
        CASE(CloseParan)
        CASE(Plus )
        CASE(Minus)
        CASE(Mul)
        CASE(Divide)
        CASE(Assign)
        CASE(Less )
        CASE(Equiv)
        CASE(NotEquiv)
        CASE(Greater)
        CASE(LessEquiv)
        CASE(GreaterEquiv)
        CASE(Not)
        CASE(And)
        CASE(Or)
        CASE(Xor)
        CASE(OpenSquareBracket)
        CASE(CloseSquareBracket)
        CASE(Increment)
        CASE(Decrement)
        CASE(BitAnd)
        CASE(BitOr)
        CASE(BitXor)
        CASE(Self)
        CASE(Lookup)
        CASE(Tab)
        CASE(NewLine)
        CASE(Fun)
        CASE(Comment)
        CASE(Yield)
        CASE(Suspend)
        CASE(Replace)
        CASE(Resume)
        CASE(PlusAssign)
        CASE(MinusAssign)
        CASE(MulAssign)
        CASE(DivAssign)
        CASE(In)
        CASE(Assert)
        CASE(ToPi)
        CASE(PiSequence)
        CASE(Debug)
        CASE(Quote)
        CASE(Sep)
        CASE(Pathname)
    }

    static char buff[BUFSIZ];
    sprintf(buff, "Unnamed LispToken %d", val);
    return buff;
}

std::ostream &operator<<(std::ostream &out, LispToken const &node)
{
    if (node.type == LispTokenEnumType::None)
        return out;

    out << LispTokenEnumType::ToString(node.type);
    switch (node.type)
    {
    case LispTokenEnumType::Int:
    case LispTokenEnumType::String:
    case LispTokenEnumType::Ident:
        out << "=" << node.Text();
    }

    return out;
}

KAI_END
