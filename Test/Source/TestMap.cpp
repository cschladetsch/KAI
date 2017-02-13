#include "./Common.h"
#include "KAI/Core/Object/GetStorageBase.h"
#include "KAI/Core/StringStreamTraits.h"

USING_NAMESPACE_KAI

class TestMap : public KAITestClass
{
protected:
	void AddrequiredClasses() override
	{
		reg.AddClass<Map>();
		reg.AddClass<StringStream>();
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
	Pointer<Map> m0 = reg.New<Map>();
	Pointer<Map> m1 = reg.New<Map>();

	Object n = reg.New(42);
	Pointer<String> s0 = reg.New<String>("World");
	Pointer<String> s1 = reg.New<String>("World");

	// make two value-identical maps (but with different value objects)
	m0->Insert(n, s0);
	m1->Insert(n, s1);

	// get a reference to the class for maps to make things easier to type
	ClassBase const &k = *m0.GetClass();

	// test that both maps are the same size and have same set of key/value pairs (by object value)
	ASSERT_TRUE(k.Equiv(m0, m1));
	
	// change the value that the second map has for it's key valued 42
	*s1 = "Hello";
	ASSERT_FALSE(k.Equiv(m0, m1));

	// and just to prove that wasn't a fluke:
	*s1 = "World";
	ASSERT_TRUE(k.Equiv(m0, m1));
}

TEST_F(TestMap, TestStringStream)
{
	Pointer<Map> m = reg.New<Map>();
	Pointer<int> n = reg.New(42);
	Pointer<String> s = reg.New<String>("Hello");

	m->Insert(n, s);

	// TODO: this test is broken.
	// it *should* work - but inserting a map into a StringStream faults
	// and I haven't looked into it yet.
	ASSERT_TRUE(false);

	// make a new string stream, insert the map into it
	Pointer<StringStream> t = reg.New<StringStream>();
	*t << m;
	
	// here, we extract a map back out of the stream.
	//
	// not happy about this approach. we have to make the map with a registry, before extracting
	// it from a StringStream.
	// It seems we should be able to just say:
	//		Object q;
	//		*t >> q;
	//		ASSERT_TRUE(q.GetClass().GetType() == Type::Traits<Map>::Number);
	Pointer<Map> m1 = reg.New<Map>();
	*t >> m1;

	// ensure the extracted map is the same value as the original map
	ASSERT_EQ(*m, *m1);
}

TEST_F(TestMap, TestBinaryStream)
{
}
