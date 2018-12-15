#include "TestCommon.h"

using namespace kai;
using namespace std;

TEST(TestPathname, TestBasic)
{
    Pathname p0("/");
    ASSERT_EQ(p0.ToString(), "/");
    Pathname p1("'/");
    ASSERT_EQ(p1.ToString(), "'/");

    Pathname p2("/Bin");
    ASSERT_EQ(p0.ToString(), "/Bin");
}

TEST(TestPathname, TestAdd)
{
    ASSERT_TRUE(false);
}

