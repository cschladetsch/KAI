

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if defined(KAI_UNIT_TESTS) && !defined(KAI_TESTS_METHOD_H)
#	define KAI_TESTS_METHOD_H

KAI_TESTS_BEGIN

struct TestMethod : Test::Suite<TestMethod>
{
	TestMethod()
	{
		Builder(this, "TestMethod")
			("TestConstruction", &TestMethod::TestConstruction)
			;
	}
	void TestConstruction();
};

KAI_TESTS_END

#endif // KAI_TESTS_METHOD_H

//EOF
