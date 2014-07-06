
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if defined(KAI_UNIT_TESTS) && !defined(KAI_TESTS_EXECUTOR_H)
#	define KAI_TESTS_EXECUTOR_H

KAI_TESTS_BEGIN

struct TestExecutor : Test::Suite<TestExecutor>
{
	TestExecutor()
	{
		Builder(this, "TestExecutor")
			("TestConstruction", &TestExecutor::TestConstruction)
			("TestStringLiterals", &TestExecutor::TestStringLiterals)
			("TestContinuations", &TestExecutor::TestContinuations)
			;
	}
	void Pre();
	void Post();

	void TestConstruction();
	void TestStringLiterals();
	void TestContinuations();

private:
	Registry registry;
	Pointer<Executor> executor;
	Pointer<Compiler> compiler;
	Pointer<Continuation> continuation;
};

KAI_TESTS_END

#endif // KAI_TESTS_EXECUTOR_H

//EOF
