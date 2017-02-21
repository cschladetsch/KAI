#include "TestCommon.h"

KAI_BEGIN

void TestCommon::SetUp()
{
	reg.AddClass<void>();
	reg.AddClass<bool>();
	reg.AddClass<int>();
	reg.AddClass<float>();
	reg.AddClass<String>();
	reg.AddClass<StringStream>();
	reg.AddClass<BinaryStream>();

	auto root = reg.New<void>();
	tree.SetRoot(root);
	reg.SetTree(tree);

	AddrequiredClasses();
}

void TestCommon::AddrequiredClasses() 
{
}

void TestCommon::TearDown()
{
	reg.Clear();
}

KAI_END

