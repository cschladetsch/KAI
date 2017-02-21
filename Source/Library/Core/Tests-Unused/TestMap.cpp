
#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include "KAI/Tests/TestMap.h"

KAI_TESTS_BEGIN

void TestMap::Pre()
{
	registry.AddClass<Map>("map");
	registry.AddClass<int>("int");
	registry.AddClass<bool>("bool");
	registry.AddClass<String>("String");
	registry.AddClass<BinaryStream>("BinaryStream");
}

void TestMap::Post()
{
	registry.ClearInstances();
}

void TestMap::TestUse()
{
	Pointer<Map> map = registry.New<Map>();
	Pointer<int> N = registry.New(42);
	Pointer<String> S = registry.New(String("Hello"));
	Pointer<bool> B = registry.New(false);

	KAI_TEST_EQUIV(map->Size(), 0);
	KAI_TEST_TRUE(map->Empty());

	map->Insert(N, S);
	map->Insert(S, B);

	KAI_TEST_FALSE(map->Empty());
	KAI_TEST_EQUIV(map->Size(), 2);

	Object Q;
	Q = map->Find(registry.New(String("Hello")));
	KAI_TEST_TRUE(Q.Valid());
	KAI_TEST_EQUIV(Q.GetTypeNumber(), Type::Traits<bool>::Number);
	KAI_TEST_EQUIV(ConstDeref<bool>(Q), false);

	Q = map->Find(registry.New(42));
	KAI_TEST_TRUE(Q.Valid());
	KAI_TEST_EQUIV(Q.GetTypeNumber(), Type::Traits<String>::Number);
	KAI_TEST_EQUIV(ConstDeref<String>(Q), "Hello");

	map->Clear();
	KAI_TEST_TRUE(map->Empty());
	KAI_TEST_EQUIV(map->Size(), 0);
}

void TestMap::TestPersistence()
{
	Pointer<Map> map = registry.New<Map>();
	Pointer<int> N = registry.New(42);
	Pointer<String> S = registry.New(String("Hello"));
	Pointer<bool> B = registry.New(false);

	map->Insert(N, S);
	map->Insert(S, B);

	BinaryStream P;
	P.SetRegistry(&registry);
	P << map;
	Object Q;
	P >> Q;
	
	KAI_TEST_EQUIV(Q, map);
}

void TestMap::TestHashUse()
{
}

void TestMap::TestHashPersistence()
{
}


KAI_TESTS_END

#endif

//EOF
