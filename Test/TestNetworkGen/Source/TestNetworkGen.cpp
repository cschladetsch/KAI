#include "Main.h"

struct MyTest : ::testing::Test
{
	virtual void SetUp() override
	{
	}

	virtual void TearDown() override
	{
	}
};

TEST_F(MyTest, Test)
{
}
