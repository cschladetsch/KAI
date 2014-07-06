
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if defined(KAI_UNIT_TESTS) && !defined(PREIX_TESTS_MAP_H)
#	define PREIX_TESTS_MAP_H

KAI_TESTS_BEGIN

struct TestMap : Test::Suite<TestMap>
{
	TestMap()
	{
		Builder(this, "TestMap")
			("TestUse", &TestMap::TestUse)
			("TestPersistence", &TestMap::TestPersistence)
			("TestHashUse", &TestMap::TestHashUse)
			("TestHashPersistence", &TestMap::TestHashPersistence)
			;
	}
	void Pre();
	void Post();
	void TestUse();
	void TestPersistence();
	void TestHashUse();
	void TestHashPersistence();

private:
	Registry registry;
};

KAI_TESTS_END

#endif // PREIX_TESTS_MAP_H

//EOF
