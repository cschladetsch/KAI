
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_TESTS_NAME_H
#	define KAI_TESTS_NAME_H

KAI_TESTS_BEGIN

struct TestDerivation : Test::Suite<TestDerivation>
{
	TestDerivation()
	{
		Builder(this, "TestDerivation")
			("TestConstruction", &TestDerivation::TestConstruction)
			;
	}
	void Pre();
	void Post();

	void TestConstruction();
};

KAI_TESTS_END

#endif // KAI_TESTS_NAME_H

//EOF
