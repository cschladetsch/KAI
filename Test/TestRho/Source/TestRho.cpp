#include "Main.h"

using namespace kai;
using namespace std;

TEST(TestRho, TestAssignment)
{
	Console console;
	console.SetLanguage(Language::Rho);
	console.Execute("a=1;");

	Object root = console.GetRoot();
	ASSERT_TRUE(root.HasChild("a"));
	ASSERT_EQ(Deref<int>(root.GetChild("a")), 3);
}

