

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if defined(KAI_UNIT_TESTS) && !defined(KAI_TESTS_COMPILER_H)
#	define KAI_TESTS_COMPILER_H

KAI_TESTS_BEGIN

struct TestCompiler : Test::Suite<TestCompiler>
{
	TestCompiler()
	{
		Builder(this, "TestCompiler")
			("TestConstruction", &TestCompiler::TestConstruction)
			("TestComments", &TestCompiler::TestComments)
			("TestFloats", &TestCompiler::TestFloats)
			;
	}
	void Pre();
	void Post();

	void TestConstruction();
	void TestStringLiterals();
	void TestContinuations();
	void TestComments();
	void TestFloats();
};

KAI_TESTS_END

#endif // KAI_TESTS_COMPILER_H

//EOF
