

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_TESTS_OBJECT_INDEXING_H
#	define KAI_TESTS_OBJECT_INDEXING_H

KAI_TESTS_BEGIN

struct TestObjectIndexing : Test::Suite<TestObjectIndexing>
{
	TestObjectIndexing()
	{
		Builder(this, "TestObjectIndexing")
			("TestConstruction", &TestObjectIndexing::Test)
			;
	}
	void Pre();
	void Post();

	void Test();
};

KAI_TESTS_END

#endif // KAI_TESTS_OBJECT_INDEXING_H

//EOF
