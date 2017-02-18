#include "TestCommon.h"

USING_NAMESPACE_KAI

struct TestExecutor : TestCommon
{
};

TEST_F(TestExecutor, TestFirst)
{
	ASSERT_EQ(1,1);
}
