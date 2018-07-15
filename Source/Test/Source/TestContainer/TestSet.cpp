#include "TestCommon.h"
#include <KAI/Core/BuiltinTypes/Set.h>

USING_NAMESPACE_KAI

class TestSet : public TestCommon
{
protected:
	void AddrequiredClasses() override
	{
		Reg().AddClass<ObjectSet>();
		Reg().AddClass<StringStream>();
		Reg().AddClass<BinaryStream>();
	}
};

TEST_F(TestSet, TestCreation)
{
}

TEST_F(TestSet, TestInsertDelete)
{
	Pointer<ObjectSet> set = _reg->New<ObjectSet>();
	_root.Set("set", set);

	Object n = _reg->New(42);
	set->Insert(n);
	Reg().GarbageCollect();

	ASSERT_TRUE(set.Exists());
	ASSERT_TRUE(n.Exists());

	set->Erase(n);
	Reg().GarbageCollect();

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
