#include "./Common.h"

KAI_BEGIN

void KAITestClass::SetUp()
{
	reg.AddClass<void>();
	reg.AddClass<bool>();
	reg.AddClass<int>();
	reg.AddClass<String>();

	auto root = reg.New<void>();
	tree.SetRoot(root);
	reg.SetTree(tree);

	AddrequiredClasses();
}

void KAITestClass::AddrequiredClasses() 
{
}

void KAITestClass::TearDown()
{
	reg.Clear();
}

KAI_END

