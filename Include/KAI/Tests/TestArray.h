

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_TESTS_ARRAY_H
#	define KAI_TESTS_ARRAY_H

KAI_TESTS_BEGIN

struct TestArray : Test::Suite<TestArray>
{
	TestArray()
	{
		Builder(this, "TestArray")
			("TestConstruction", &TestArray::TestConstruction)
			;
	}
	void Pre();
	void Post();

	void TestConstruction();
};

KAI_TESTS_END

#endif // KAI_TESTS_ARRAY_H

//EOF
