

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if defined(KAI_UNIT_TESTS) && !defined(KAI_TESTS_PATHNAME_H)
#	define KAI_TESTS_PATHNAME_H

KAI_TESTS_BEGIN

struct TestPathname : Test::Suite<TestPathname>
{
	TestPathname()
	{
		Builder(this, "TestPathname")
			("TestConstruction", &TestPathname::TestConstruction)
			;
	}
	void TestConstruction();
};

KAI_TESTS_END

#endif // KAI_TESTS_PATHNAME_H

//EOF
