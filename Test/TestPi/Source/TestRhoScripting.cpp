#include "TestBase.h"
#include "KAI/Console.h"

using namespace kai;
using namespace std;

TEST(TestPiScripting, TestAssignment)
{
	Console console;
	console.SetLanguage(Language::Pi);
	console.Execute("'a 1 :=");

	Object root = console.GetRoot();
	ASSERT_TRUE(root.HasChild("a"));
	ASSERT_EQ(Deref<int>(root.GetChild("a")), 3);
}

