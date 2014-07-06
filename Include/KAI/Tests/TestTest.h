
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_TESTS_TEST_TEST_H
#	define KAI_TESTS_TEST_TEST_H

KAI_TESTS_BEGIN

struct TestTest : Test::Suite<TestTest>
{
	TestTest()
	{
		Builder(this, "TestTest")
			("TestMethod0", &TestTest::TestMethod0)
			;
	}

	void Pre();
	void Post();

	void TestMethod0();
};

KAI_TESTS_END

#endif // KAI_TESTS_TEST_TEST_H

//EOF
