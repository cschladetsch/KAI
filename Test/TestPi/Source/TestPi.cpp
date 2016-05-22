#include "Main.h"

using namespace kai;
using namespace std;

TEST(TestPi, TestAssignment)
{
	Console console;
	console.SetLanguage(Language::Pi);
	console.Execute("'a 1 :=");

	Object root = console.GetRoot();
	Label name("a");
	ASSERT_TRUE(root.HasChild(name));
	ASSERT_EQ(Deref<int>(root.GetChild(name)), 3);
}

