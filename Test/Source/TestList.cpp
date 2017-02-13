#include "./Common.h"


USING_NAMESPACE_KAI

class TestList : public KAITestClass
{
protected:
	void AddrequiredClasses() override
	{
		reg.AddClass<List>();
		reg.AddClass<StringStream>();
		reg.AddClass<BinaryStream>();
	}
};

TEST_F(TestList, TestCreation)
{
	Pointer<List> list = reg.New<List>();
	ASSERT_TRUE(list.Exists());
	ASSERT_TRUE(list->Size() == 0);
	ASSERT_TRUE(list->Empty());

	reg.GarbageCollect();

	ASSERT_FALSE(list.Exists());
}

TEST_F(TestList, TestInsertDelete)
{
	Pointer<List> list = reg.New<List>();
	tree.GetRoot().Set("list", list);

	Object n = reg.New(42);
	list->PushBack(n);
	reg.GarbageCollect();

	ASSERT_TRUE(list.Exists());
	ASSERT_TRUE(n.Exists());

	list->Erase(n);
	reg.GarbageCollect();

	ASSERT_TRUE(list.Exists());
	ASSERT_FALSE(n.Exists());
}

TEST_F(TestList, TestComparison)
{
}

TEST_F(TestList, TestStringStream)
{
}

TEST_F(TestList, TestBinaryStream)
{
}
