#include "TestCommon.h"

USING_NAMESPACE_KAI

struct StringTest : TestCommon
{
	void AddrequiredClasses() override
	{
		Reg().AddClass<bool>();
		Reg().AddClass<int>();
		Reg().AddClass<String>();
	}
};

TEST_F(StringTest, TestBoolean)
{
	Pointer<String> s0 = Reg().New<String>("foo");
	ASSERT_TRUE(s0.GetClass()->Boolean(s0.GetStorageBase()));
	*s0 = "";
	ASSERT_FALSE(s0.GetClass()->Boolean(s0.GetStorageBase()));
}

TEST_F(StringTest, TestCompare)
{
	Pointer<String> s0 = Reg().New<String>("a");
	Pointer<String> s1 = Reg().New<String>("a");

	ASSERT_TRUE(s0.GetClass()->Equiv(s0.GetStorageBase(), s1.GetStorageBase()));

	*s1 = "b";
	ASSERT_FALSE(s0.GetClass()->Equiv(s0.GetStorageBase(), s1.GetStorageBase()));
	ASSERT_TRUE(s0.GetClass()->Less(s0.GetStorageBase(), s1.GetStorageBase()));
}
