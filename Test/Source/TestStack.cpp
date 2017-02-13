#include "./Common.h"

USING_NAMESPACE_KAI
class TestStack : public KAITestClass
{
protected:
	void AddrequiredClasses() override
	{
		reg.AddClass<Stack>();
	}
};

TEST_F(TestStack, TestCreation)
{
	Pointer<Stack> stack = reg.New<Stack>();
	ASSERT_TRUE(stack.Exists());
	ASSERT_TRUE(stack->Size() == 0);
	ASSERT_TRUE(stack->Empty());

	reg.GarbageCollect();

	ASSERT_FALSE(stack.Exists());
}

TEST_F(TestStack, TestInsertDelete)
{
	Pointer<Stack> stack = reg.New<Stack>();
	tree.GetRoot().Set("stack", stack);

	Object n = reg.New(42);
	stack->Push(n);
	reg.GarbageCollect();

	ASSERT_TRUE(stack.Exists());
	ASSERT_TRUE(n.Exists());

	stack->Erase(n);
	reg.GarbageCollect();

	ASSERT_TRUE(stack.Exists());
	ASSERT_FALSE(n.Exists());
}

TEST_F(TestStack, TestComparison)
{
}

TEST_F(TestStack, TestStringStream)
{
}

TEST_F(TestStack, TestBinaryStream)
{
}

