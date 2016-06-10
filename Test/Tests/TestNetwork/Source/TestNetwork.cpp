#include "Main.h"

using namespace kai::net;

struct TestNetwork : ::testing::Test
{
	virtual void SetUp() override
	{
	}

	virtual void TearDown() override
	{
	}
};

TEST_F(TestNetwork, TestConnection)
{
	Node a, b;
	a.Connect(b);


}


