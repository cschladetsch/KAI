#include "KAI/Language/Lisp/LispAstNode.h"

KAI_BEGIN

#undef CASE
#undef CASE_LOWER
#undef CASE_REPLACE

const char *LispAstNodes::ToString(Enum ty)
{
    switch (ty)
    {
        CASE(None)
        CASE(Program)
        CASE(Ident)
        CASE(GetMember)
        CASE(Function)
        CASE(Block)
        CASE(Conditional)
        CASE(Assignment)
        CASE(Call)
        CASE(ArgList)
        CASE(IndexOp)
        CASE(ForEach)
        CASE(For)
        CASE(List)
        CASE(Map)
        CASE(Continuation)
        CASE(TokenType)
        CASE(Pathname)
    }

    static char buff[BUFSIZ];
    sprintf(buff, "Case %d", ty);
    return buff;
}

KAI_END
