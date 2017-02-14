#include "./Common.h"


USING_NAMESPACE_KAI

class TestList : public KAITestClass
{
protected:
	void AddrequiredClasses() override
	{
		reg.AddClass<List>();
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

	ASSERT_EQ(list->Size(), 1);
	ASSERT_FALSE(list->Empty());
	list->Erase(n);
	ASSERT_EQ(list->Size(), 0);
	ASSERT_TRUE(list->Empty());
	reg.GarbageCollect();

	ASSERT_TRUE(list.Exists());
	ASSERT_FALSE(n.Exists());
}

TEST_F(TestList, TestComparison)
{
	Pointer<List> l0 = reg.New<List>();
	Pointer<List> l1 = reg.New<List>();
	tree.GetRoot().Set("list0", l0);
	tree.GetRoot().Set("list1", l1);

	l0->PushBack(reg.New(42));
	l0->PushBack(reg.New<String>("Hello"));

	l1->PushBack(reg.New(42));
	l1->PushBack(reg.New<String>("Hello"));

	// assert same value
	ASSERT_EQ(*l0, *l1);

	// change the first value to be 2, which is less than 42, so it should be less than
	Deref<int>(l0->Front()) = 2;

	// assert less than
	ASSERT_LT(*l0, *l1);
}

TEST_F(TestList, TestStringStream)
{
}

TEST_F(TestList, TestBinaryStream)
{
	Pointer<List> l = reg.New<List>();
	Pointer<BinaryStream> s = reg.New<BinaryStream>();

	l->PushBack(reg.New(42));
	l->PushBack(reg.New<String>("Hello"));

	ASSERT_EQ(s->Size(), 0);

	*s << l;

	ASSERT_GT(s->Size(), 0);

	// this fails for same reason that all the other extraction
	// from binary streams fail: the underlying BinaryPacket
	// that BinaryStream derives from isn't given a registry...
	// Object l2;
	// *s >> l2;

	// ASSERT_EQ(l2, l);
}
