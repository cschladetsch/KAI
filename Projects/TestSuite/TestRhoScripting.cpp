#include "TestBase.h"
#include "KAI/Console.h"

using namespace kai;
using namespace std;

TEST(TestFunctionScripting, TestAssignment)
{
	Memory::StandardAllocator alloc;
	Console console(&alloc);

	// a registry is a factory for classes and instances
	//Registry &reg = console.GetRegistry();
	Object root = console.GetRoot();
	console.Execute("a=1;");

	ASSERT_TRUE(root.HasChild("a"));
	ASSERT_EQ(Deref<int>(root.GetChild("a")), 3);
}

