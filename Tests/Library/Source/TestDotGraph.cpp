#include "TestBase.h"
#include "KAI/Tree.h"
#include "KAI/DotGraph.h"
#include "KAI/BuiltinTypes/List.h"

using namespace kai;
using namespace std;

struct TestOwned
{
	Pointer<List> list;
	Pointer<int> num;
};

KAI_BEGIN

KAI_TYPE_TRAITS(TestOwned, 666, 0);

KAI_END

TEST(TestDotGraph, Test)
{
	Registry R;
	R.AddClass<void>();
	R.AddClass<int>();
	R.AddClass<List>();
	ClassBuilder<TestOwned>(R, "TestOwned")
		.Methods
		.Properties
		//("list", &TestOwned::list, "", CreateParams::Create)
		("num", &TestOwned::num)
		;

	Tree tree;
	Object root = R.New<void>();
	R.SetTree(tree);

	Pointer<TestOwned> to = R.New<TestOwned>();
	Pointer<int> n = R.New<int>();

	root.SetChild("test_owned", to);
	root.SetChild("n", n);

	to->list->PushBack(R.New(42));
	to->list->PushBack(R.New(123));
	to->num = R.New(456);

	String filename("\\tmp\\test.graph");
	DotGraph(root, filename);

	//KAI_TEST_TRUE(File("\\test.graph").CanRead());
}