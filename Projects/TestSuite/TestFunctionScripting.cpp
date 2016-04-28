#include "TestBase.h"
#include "KAI/KAIClassBuilder.h"
#include "KAI/Console.h"

using namespace kai;
using namespace std;

bool funCalled[5];

// just making some functions that we will also add to the runtime soon
void Function_0()
{
	funCalled[0] = true;
}

void Function_1(int n)
{
	funCalled[1] = true;
}

String Function_2(int n, float f, String p)
{
	funCalled[2] = true;
	KAI_TRACE_3(n, f, p);
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
	// we can give the KAI runtime a custom allocator to use. we will just use
	// the standard one which uses malloc and free
	Memory::StandardAllocator alloc;
	Console console(&alloc);

	// a registry is a factory for classes and instances
	Registry &reg = console.GetRegistry();
	Object root = console.GetRoot();

	// functions and methods get their arguments from and return results to a stack
	Pointer<Stack> stack;

	// add general functions to the root of the tree
	AddFunction(root, Function_0, "Function0"); 
	AddFunction(root, Function_1, "Function1"); 
	AddFunction(root, Function_2, "Function2");
	AddFunction(root, Function_3, "Function3");

	// invoke the functions; take copies of the resultant stacks after each function completes
	Pointer<Stack> stack_A = console.Execute("Function0();");
	Pointer<Stack> stack_B = console.Execute("Function1(42);");
	Pointer<Stack> stack_C = console.Execute("Function2(123, 3.0, \"bar\");");
	Pointer<Stack> stack_D = console.Execute("Function3(mystruct);");

	for (int n = 0; n < 4; ++n)
		ASSERT_TRUE(funCalled[n]);

	// show results and test
	ASSERT_TRUE(stack_A->Empty());
	ASSERT_TRUE(stack_B->Empty());
	EXPECT_EQ(ConstDeref<String>(stack_C->Pop()), "barfoo");
	EXPECT_EQ(ConstDeref<int>(stack_D->Pop()), 345);
}

