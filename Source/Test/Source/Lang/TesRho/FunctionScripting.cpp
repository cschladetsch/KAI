#include <KAI/Core/Detail/Function.h>

#include "MyTestStruct.h"

USING_NAMESPACE_KAI

using namespace std;

bool funCalled[5];

// make some functions that we will also add to the runtime
void Function_0()
{
	funCalled[0] = true;
}

void Function_1(int )
{
	funCalled[1] = true;
}

String Function_2(int n, int f, String p)
{
	funCalled[2] = true;
	// KAI_TRACE_3(n, f, p);
	return p + "foo";
}

Object Function_3(Object object)
{
	funCalled[3] = true;
	KAI_TRACE_1(object);
	return object["num"];
}

TEST(TestFunctionScripting, Test)
{
	Console console;
	console.SetLanguage(Language::Rho);
	Object root = console.GetRoot();
	Registry &reg = console.GetRegistry();
	MyStruct::Register(reg);

	Pointer<MyStruct> mystruct = reg.New<MyStruct>();
	mystruct->num = 345;
	mystruct->string = "hello world";

	console.GetTree().AddSearchPath(root);//Pathname("/"));

	// Process::trace = 10;
	root["mystruct"] = mystruct;
	AddFunction(root, Function_0, Label("Function0"));
	AddFunction(root, Function_1, Label("Function1"));
	AddFunction(root, Function_2, Label("Function2"));
	AddFunction(root, Function_3, Label("Function3"));

	// these fail because of changes to CommonLexer
	console.Execute("Function0()");
	console.Execute("Function1(42)");
	console.Execute("Function2(123, 3, \"bar\")");
	console.Execute("Function3(mystruct)");

	for (int n = 0; n < 2; ++n)
		ASSERT_TRUE(funCalled[n]);

	Value<Stack> stack = console.GetExecutor()->GetDataStack();
	EXPECT_EQ(stack->Size(), 2);
	EXPECT_EQ(ConstDeref<int>(stack->Pop()), 345);
	EXPECT_EQ(ConstDeref<String>(stack->Pop()), "barfoo");
	EXPECT_EQ(stack->Size(), 0);
}
