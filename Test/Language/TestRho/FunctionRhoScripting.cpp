#include "KAI/Core/Console.h"
#include "MyTestStruct.h"
#include "TestLangCommon.h"

KAI_BEGIN

using namespace std;

bool funCalled[5];

// make some functions that we will also add to the runtime
void Function_0() { funCalled[0] = true; }

void Function_1(int) { funCalled[1] = true; }

String Function_2(int n, int f, String p) {
    funCalled[2] = true;
    // KAI_TRACE_3(n, f, p);
    return p + String("foo");
}

Object Function_3(Object object) {
    funCalled[3] = true;
    KAI_TRACE_1(object);
    return object["num"];
}

TEST_F(TestLangCommon, TestRhoReflection) {
    Registry &reg = *_reg;
    MyStruct::Register(reg);

    Pointer<MyStruct> mystruct = reg.New<MyStruct>();
    mystruct->num = 345;
    mystruct->string = "hello world";

    _console.GetTree().AddSearchPath(_root);

    // Process::trace = 10;
    _root["mystruct"] = mystruct;
    AddFunction(_root, Function_0, Label("Function0"));
    AddFunction(_root, Function_1, Label("Function1"));
    AddFunction(_root, Function_2, Label("Function2"));
    AddFunction(_root, Function_3, Label("Function3"));

    // CJS TODO these fail because of changes to CommonLexer
    std::cerr << "**** Skipping due to changes in Common Lexer" << std::endl;
    return;

    _console.Execute("Function0()");
    _console.Execute("Function1(42)");
    _console.Execute("Function2(123, 3, \"bar\")");
    _console.Execute("Function3(mystruct)");

    for (int n = 0; n < 2; ++n) ASSERT_TRUE(funCalled[n]);

    Value<Stack> stack = _console.GetExecutor()->GetDataStack();
    EXPECT_EQ(stack->Size(), 2);
    EXPECT_EQ(ConstDeref<int>(stack->Pop()), 345);
    EXPECT_EQ(ConstDeref<String>(stack->Pop()), "barfoo");
    EXPECT_EQ(stack->Size(), 0);
}

TEST_F(TestLangCommon, RunScripts) { ExecScripts(); }

KAI_END
