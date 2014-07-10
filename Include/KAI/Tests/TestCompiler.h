
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if defined(KAI_UNIT_TESTS) && !defined(KAI_TESTS_COMPILER_H)
#	define KAI_TESTS_COMPILER_H

KAI_BEGIN
class Console;
KAI_END

KAI_TESTS_BEGIN

struct TestCompiler : Test::Suite<TestCompiler>
{
	Console *con;

	TestCompiler()
	{
		Builder(this, "TestCompiler")
			("TestConstruction", &TestCompiler::TestConstruction)
			("TestFunctions", &TestCompiler::TestFunctions)
			("TestExpressions", &TestCompiler::TestExpressions)
			("TestStatements", &TestCompiler::TestStatements)
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
	void TestExpressions();
	void TestStatements();
	void TestFloats();
	void TestFunctions();
};

KAI_TESTS_END

#endif // KAI_TESTS_COMPILER_H

//EOF
