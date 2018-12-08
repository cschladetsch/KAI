#include "TestCommon.h"
#include "KAI/Language/Hlsl/Hlsl.h"

USING_NAMESPACE_KAI

using namespace std;

TEST(TestHlsl, Test)
{
    Console console;
    Registry &reg = console.GetRegistry();
    const char *text =( "float res = exp2(pw*a) + exp2(pw*b);");
    HlslLexer lex(text, reg);
    lex.Process();
    TEST_COUT << lex.Print();
}
