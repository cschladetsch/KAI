#include <KAI/Core/Detail/Function.h>

#include "TestLangCommon.h"

using namespace kai;
using namespace std;

bool funCalled[5];

// just making some functions that we will also add to the runtime soon
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
	try
	{
		Console console;
		console.SetLanguage(Language::Rho);
		Object root = console.GetRoot();

		AddFunction(root, Function_0, Label("Function0"));
		AddFunction(root, Function_1, Label("Function1"));
		AddFunction(root, Function_2, Label("Function2"));
		AddFunction(root, Function_3, Label("Function3"));

		console.Execute("Function0()");
		console.Execute("Function1(42)");
		console.Execute("Function2(123, 3, \"bar\")");
		console.Execute("Function3(mystruct)");

		for (int n = 0; n < 2; ++n)
			ASSERT_TRUE(funCalled[n]);

		Value<Stack> stack = console.GetExecutor()->GetDataStack();
		EXPECT_EQ(stack->Size(), 0);
		for (auto val : *stack)
		{
			KAI_TRACE_1(val);
		}

		// // show results and
		// ASSERT_TRUE(stack_A->Empty());
		// ASSERT_TRUE(stack_B->Empty());
		// EXPECT_EQ(ConstDeref<String>(stack_C->Pop()), "barfoo");
		// EXPECT_EQ(ConstDeref<int>(stack_D->Pop()), 345);
	}
	catch (kai::Exception::Base &e)
	{
		cerr << e.ToString() << endl;
	}
	catch (std::exception &e)
	{
		cerr << e.what() << endl;
	}
}

