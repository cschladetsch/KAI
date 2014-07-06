
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if defined(KAI_UNIT_TESTS) && !defined(KAI_TESTS_TREE_H)
#	define KAI_TESTS_TREE_H

KAI_TESTS_BEGIN

struct TestTree : Test::Suite<TestTree>
{
	TestTree()
	{
		Builder(this, "TestTree")
			("TestConstruction", &TestTree::TestConstruction)
			("TestStore", &TestTree::TestStore)
			("TestRetreive", &TestTree::TestRetreive)
			("TestScope", &TestTree::TestScope)
			("TestSearch", &TestTree::TestSearch)
			;
	}
	void Pre();
	void Post();

	void TestConstruction();
	void TestStore();
	void TestRetreive();
	void TestScope();
	void TestSearch();
};

KAI_TESTS_END

#endif // KAI_TESTS_TREE_H

//EOF
