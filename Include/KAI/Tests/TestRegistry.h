
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if defined(KAI_UNIT_TESTS) && !defined(KAI_TESTS_REGISTRY_H)
#	define KAI_TESTS_REGISTRY_H

KAI_TESTS_BEGIN

struct TestRegistry : Test::Suite<TestRegistry>
{
	TestRegistry();

	void TestConstruction();
	void TestMarkAndSweep();
	void TestPropertyMarkingPersistence();
	void TestPropertyMarkingRemoval();
	void TestContainers();
	void TestMarkAndSweepContainers();
};

KAI_TESTS_END

#endif // KAI_TEST_REGISTRY_H

//EOF
