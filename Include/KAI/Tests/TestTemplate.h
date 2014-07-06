


#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_TESTS_NAME_H
#	define KAI_TESTS_NAME_H

KAI_TESTS_BEGIN

struct TestName : Test::Suite<TestName>
{
	TestName()
	{
		Builder(this, "TestName")
			("TestConstruction", &TestName::Test)
			;
	}
	void Pre();
	void Post();

	void Test();
};

KAI_TESTS_END

#endif // KAI_TESTS_NAME_H

//EOF
