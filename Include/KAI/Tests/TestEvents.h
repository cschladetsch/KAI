
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_TEST_EVENTS_H
#	define KAI_TEST_EVENTS_H

KAI_TESTS_BEGIN

struct TestEvents : Test::Suite<TestEvents>
{
	TestEvents()
	{
		Builder(this, "TestEvents")
			("TestConstruction", &TestEvents::TestConstruction)
			;
	}
	void Pre();
	void Post();

	void TestConstruction();
};

KAI_TESTS_END

#endif // KAI_TEST_EVENTS_H

//EOF
