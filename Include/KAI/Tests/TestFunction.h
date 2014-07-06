
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if defined(KAI_UNIT_TESTS) && !defined(KAI_TESTS_FUNCTION_H)
#	define KAI_TESTS_FUNCTION_H

KAI_TESTS_BEGIN

struct TestFunction : Test::Suite<TestFunction>
{
	TestFunction()
	{
		Builder(this, "TestFunction")
			("TestConstruction", &TestFunction::TestConstruction)
			("TestExecutor", &TestFunction::TestExecutor)
			;
	}
	void Pre();
	void Post();
	void TestConstruction();
	void TestExecutor();

private:
	Registry registry;
	Value<Stack> stack;
};

KAI_TESTS_END

#endif // KAI_TESTS_METHOD_H

//EOF
