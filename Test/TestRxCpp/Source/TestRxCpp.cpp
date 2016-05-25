#include "Main.h"

struct MyTest : ::testing::test
{
	virtual void SetUp() override
	{
	}

	virtual void Teardown() override
	{
	}
};

TEST_F(MyTest, Test)
{
}