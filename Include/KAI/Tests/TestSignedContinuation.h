
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_TESTS_TEST_SIGNED_CONTINUATION_H
#	define KAI_TESTS_TEST_SIGNED_CONTINUATION_H

KAI_TESTS_BEGIN

struct TestSignedContinuation : Test::Suite<TestSignedContinuation>
{
	TestSignedContinuation()
	{
		Builder(this, "TestSignedContinuation")
			("TestConstruction", &TestSignedContinuation::TestConstruction)
			;
	}
	void Pre();
	void Post();

	void TestConstruction();
};

KAI_TESTS_END

#endif // KAI_TESTS_TEST_SIGNED_CONTINUATION_H

//EOF
