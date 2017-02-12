#include "Test/Common.h"
#include <KAI/Core/Tree.h>
#include <KAI/Core/DotGraph.h>

USING_NAMESPACE_KAI

class TestRegistry : public ::testing::Test
{
public:
	Tree tree;
	Registry reg;

protected:
	virtual void SetUp() override
	{
		reg.AddClass<void>();
		auto root = reg.New<void>();
		tree.SetRoot(root);
		reg.SetTree(tree);

		reg.AddClass<int>();
		reg.AddClass<bool>();
		reg.AddClass<String>();
		reg.AddClass<List>();
		// _reg.AddClass<StringStream>();
	}	

	virtual void TearDown() override
	{
		// _reg.Clear();
	}
};

TEST_F(TestRegistry, TestInit)
{
	ASSERT_EQ(42, 42);
	ASSERT_FALSE(1 == 0);
}

TEST_F(TestRegistry, TestAddClass)
{
	ASSERT_EQ(42, 42);
	ASSERT_FALSE(1 == 0);
}

TEST_F(TestRegistry, TestVectorOwnership)
{
}

TEST_F(TestRegistry, TestListOwnership)
{
}

TEST_F(TestRegistry, TestMapOwnership)
{
}

TEST_F(TestRegistry, TestNestedFields)
{
}

TEST_F(TestRegistry, TestNestedConainerListFields)
{
}

TEST_F(TestRegistry, TestNestedConainerVectorFields)
{
}

TEST_F(TestRegistry, TestNestedConainerMapFields)
{
}

TEST_F(TestRegistry, TestRemoveClasses)
{
}

TEST_F(TestRegistry, TestSimpleDotGraph)
{
	auto root = tree.GetRoot();
	Pointer<List> list = reg.New<List>();
	auto num = reg.New<int>(42);
	auto str = reg.New<String>("Hello");

	list->PushBack(num);
	list->PushBack(str);
	root.Set("list", list);

	DotGraph(reg.GetTree()->GetRoot(), "RegTest1");
}
