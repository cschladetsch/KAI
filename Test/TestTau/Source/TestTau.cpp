#include  "Main.h"

struct TestTau : ::testing::Test
{
	virtual void SetUp() override
	{
	}

	virtual void TearDown() override
	{
	}
};

TEST_F(TestTau, Test)
{
}
