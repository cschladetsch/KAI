#pragma once

#include <KAI/Language/Hlsl/HlslToken.h>
#include <KAI/Language/Common/LexerCommon.h>

KAI_BEGIN

struct HlslLexer : LexerCommon<HlslTokenEnumType>
{
    typedef LexerCommon<HlslTokenEnumType> Parent;
    typedef TokenBase<HlslTokenEnumType> TokenNode;

    HlslLexer(const char *text, Registry &r) : Parent(text, r) { }

    void AddKeyWords() override;
    bool NextToken() override;
    void Terminate() override;

protected:
    bool LexPathname();
    using Parent::_reg;
};

KAI_END

