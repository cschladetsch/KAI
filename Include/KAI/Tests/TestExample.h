
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_TESTS_EXAMPLE_H
#	define KAI_TESTS_EXAMPLE_H

KAI_TESTS_BEGIN

struct TestExample : Test::Suite<TestExample>
{
	TestExample()
	{
		Builder(this, "TestExample")
			("TestStruct", &TestExample::TestStruct)
			;
	}
	void Pre();
	void Post();

	void TestStruct();
};

KAI_TESTS_END

#endif // KAI_TESTS_NAME_H

//EOF
