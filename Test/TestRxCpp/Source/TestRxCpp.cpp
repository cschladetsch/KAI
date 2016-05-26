 #include "Main.h"

namespace rx = rxcpp;
using namespace std;

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
	// I have no idea what I am doing with RxCpp
}
