#include <KAI/Core/BuiltinTypes/Set.h>

#include "TestCommon.h"

USING_NAMESPACE_KAI

class TestSet : public TestCommon
{
protected:
	void AddrequiredClasses() override
	{
		reg.AddClass<ObjectSet>();
		reg.AddClass<StringStream>();
		reg.AddClass<BinaryStream>();
	}
};

TEST_F(TestSet, TestCreation)
{
}

TEST_F(TestSet, TestInsertDelete)
{
	Pointer<ObjectSet> set = reg.New<ObjectSet>();
	tree.GetRoot().Set("set", set);

	Object n = reg.New(42);
	set->Insert(n);
	reg.GarbageCollect();

	ASSERT_TRUE(set.Exists());
	ASSERT_TRUE(n.Exists());

	set->Erase(n);
	reg.GarbageCollect();

	ASSERT_TRUE(set.Exists());
	ASSERT_FALSE(n.Exists());
}

TEST_F(TestSet, TestOwnership)
{
}

TEST_F(TestSet, TestComparison)
{
}

TEST_F(TestSet, TestStringStream)
{
}

TEST_F(TestSet, TestBinaryStream)
{
}
