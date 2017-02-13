#include "./Common.h"

USING_NAMESPACE_KAI

struct TestExecutor : KAITestClass 
{
};

TEST_F(TestExecutor, TestFirst)
{
	ASSERT_EQ(1,1);
}
