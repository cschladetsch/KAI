

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_TESTS_CLOSURE_H
#	define KAI_TESTS_CLOSURE_H

KAI_TESTS_BEGIN

struct TestClosure : Test::Suite<TestClosure>
{
	TestClosure()
	{
		Builder(this, "TestClosure")
			("TestConstruction", &TestClosure::Test)
			;
	}
	void Pre();
	void Post();

	void Test();
};

KAI_TESTS_END

#endif // KAI_TESTS_CLOSURE_H

//EOF
