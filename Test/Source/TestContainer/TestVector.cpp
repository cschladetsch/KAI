// WHY THE FUCK did I call this an Array
#include <KAI/Core/BuiltinTypes/Array.h>

#include "TestCommon.h"

USING_NAMESPACE_KAI

class TestArray : public TestCommon
{
protected:
	void AddrequiredClasses() override
	{
		reg.AddClass<Array>();
	}
};

TEST_F(TestArray, TestCreation)
{
	Pointer<Array> cont = reg.New<Array>();
	ASSERT_TRUE(cont.Exists());
	ASSERT_TRUE(cont->Size() == 0);
	ASSERT_TRUE(cont->Empty());

	reg.GarbageCollect();

	ASSERT_FALSE(cont.Exists());
}

TEST_F(TestArray, TestInsertDelete)
{
	Pointer<Array> cont = reg.New<Array>();
	tree.GetRoot().Set("cont", cont);

	Object n = reg.New(42);
	cont->PushBack(n);
	reg.GarbageCollect();

	ASSERT_TRUE(cont.Exists());
	ASSERT_TRUE(n.Exists());

	cont->Erase(n);
	reg.GarbageCollect();

	ASSERT_TRUE(cont.Exists());
	ASSERT_FALSE(n.Exists());
}

TEST_F(TestArray, TestComparison)
{
}

TEST_F(TestArray, TestStringStream)
{
}

TEST_F(TestArray, TestBinaryStream)
{
}
