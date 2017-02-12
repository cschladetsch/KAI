#include "./Main.h"
#include <KAI/Core/BuiltinTypes.h>

USING_NAMESPACE_KAI

class TestRegistry : public ::testing::Test
{
	Registry _reg;

protected:
	virtual void SetUp() override
	{
		_reg.AddClass<void>();
		_reg.AddClass<int>();
		_reg.AddClass<bool>();
		_reg.AddClass<String>();
		// _reg.AddClass<StringStream>();
	}	

	virtual void TearDown() override
	{
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

