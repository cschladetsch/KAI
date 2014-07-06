
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_TESTS_TRICOLOR_H
#	define KAI_TESTS_TRICOLOR_H

KAI_TESTS_BEGIN

struct TestTriColor: Test::Suite<TestTriColor>
{
	TestTriColor()
	{
		test_number = 0;
		Builder(this, "TestTriColor")
			("Test", &TestTriColor::Test)
			("TestNested", &TestTriColor::TestNested)
			("TestMultipleReferences", &TestTriColor::TestMultipleReferences)
			("TestOverride", &TestTriColor::TestOverride)
			("TestContainers", &TestTriColor::TestContainers)
			;
	}
	void Pre();
	void Post();

	void Test();
	void TestNested();
	void TestMultipleReferences();
	void TestOverride();
	void TestContainers();

	void TriColor(Registry &);

	int test_number;
	Registry *registry;
	Tree *tree;
};

KAI_TESTS_END

#endif // KAI_TESTS_TRICOLOR_H

//EOF
