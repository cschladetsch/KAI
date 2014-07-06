
#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include "KAI/Tree.h"
#	include "KAI/DotGraph.h"
#	include "KAI/BuiltinTypes/List.h"
#	include "KAI/Tests/TestDotGraph.h"

KAI_BEGIN

struct TestOwned
{
	Pointer<List> list;
	Pointer<int> num;
};

KAI_TYPE_TRAITS(TestOwned, 666, Type::Properties::None);

KAI_END

KAI_TESTS_BEGIN

void TestDotGraph::Test()
{
	Registry R;
	R.AddClass<void>();
	R.AddClass<int>();
	R.AddClass<List>();
	ClassBuilder<TestOwned>(R, "TestOwned")
		.Methods
		.Properties
			("list", &TestOwned::list, "", CreateParams::Create)
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

	String filename("\\test.graph");
	DotGraph(root, filename);

	//KAI_TEST_TRUE(File("\\test.graph").CanRead());
}

void TestDotGraph::Pre()
{
}

void TestDotGraph::Post()
{
}

KAI_TESTS_END

#endif // KAI_UNIT_TESTS

//EOF
