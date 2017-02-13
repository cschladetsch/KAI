#include "./Common.h"
#include "KAI/Core/Object/GetStorageBase.h"


USING_NAMESPACE_KAI

class TestMap : public KAITestClass
{
protected:
	void AddrequiredClasses() override
	{
		reg.AddClass<Map>();
	}
};

TEST_F(TestMap, TestCreation)
{
	Pointer<Map> map = reg.New<Map>();
	ASSERT_TRUE(map.Exists());
	ASSERT_TRUE(map->Size() == 0);
	ASSERT_TRUE(map->Empty());

	reg.GarbageCollect();

	ASSERT_FALSE(map.Exists());
}

TEST_F(TestMap, TestInsertDelete)
{
	Pointer<Map> map = reg.New<Map>();
	Pointer<Map> dangling = reg.New<Map>();
	
	// add the map to the root of the object tree, so it can be
	// found and hence not GC'd
	tree.GetRoot().Set("map", map);
	reg.GarbageCollect();

	// the map will still exist after the GC - but the `dangling` map
	// won't because it can't be reached by the Registry reg
	ASSERT_TRUE(map.Exists());
	ASSERT_FALSE(dangling.Exists());

	// make a key and a value to insert into map
	Pointer<int> n = reg.New(42);
	Pointer<String> s = reg.New<String>("Hello");

	map->Insert(n, s);

	ASSERT_TRUE(map->ContainsKey(n));
	Object found = map->GetValue(n);

	ASSERT_STREQ(ConstDeref<String>(found).c_str(), "Hello");
	ASSERT_EQ(found.GetHandle(), s.GetHandle());

	ASSERT_TRUE(map.Exists());
	ASSERT_TRUE(n.Exists());
	ASSERT_TRUE(found.Exists());

	// by removing the key associated with n, we also remove the value
	map->Erase(n);
	reg.GarbageCollect();

	// now, neither the key nor value should exist,
	// but the map itself should exist because it was added
	// to the root of the tree above
	ASSERT_TRUE(map.Exists());
	ASSERT_FALSE(n.Exists());
	ASSERT_FALSE(found.Exists());
}

TEST_F(TestMap, TestComparison)
{
}

TEST_F(TestMap, TestStringStream)
{
}

TEST_F(TestMap, TestBinaryStream)
{
}
