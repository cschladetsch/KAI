
#include "KAI/Test/Base.h"
#	include "KAI/Tree.h"
#	include "KAI/Test/Base.h"
#	include "KAI/BuiltinTypes/List.h"
#	include "KAI/Tests/TestTriColor.h"
#	include "KAI/DotGraph.h"

KAI_BEGIN

struct TriColorStruct
{
	Pointer<List> contained;
	Object other;
};

KAI_TYPE_TRAITS(TriColorStruct, 666, Type::Properties::None);

KAI_END

KAI_TESTS_BEGIN

void TestTriColor::Test()
{ 
	Registry R;
	Tree tree;
	ClassBuilder<TriColorStruct>(R, "TriColorStruct")
		.Methods
		.Properties
			("contained", &TriColorStruct::contained, "", CreateParams::Create)
			("other", &TriColorStruct::other)
		;
	R.AddClass<void>();
	R.AddClass<Object>();
	R.AddClass<int>();
	List::Register(R);

	Object root = R.New<void>();
	tree.SetRoot(root);
	R.SetTree(tree);

	Object N = R.New<int>();
	KAI_TEST_TRUE(N.Exists());

	R.TriColor();
	KAI_TEST_FALSE(N.Exists());

	N = R.New<int>();
	Object M = R.New<int>();
	N.SetChild("M", M);
	root.SetChild("N", N);

	KAI_TEST_TRUE(root.Exists());
	KAI_TEST_TRUE(N.Exists());
	KAI_TEST_TRUE(M.Exists());
	
	R.TriColor();
	KAI_TEST_TRUE(root.Exists());
	KAI_TEST_TRUE(N.Exists());
	KAI_TEST_TRUE(M.Exists());

	root.RemoveChild("N");
	R.TriColor();
	KAI_TEST_TRUE(root.Exists());
	KAI_TEST_FALSE(N.Exists());
	KAI_TEST_FALSE(M.Exists());

	Pointer<TriColorStruct> tcs = R.New<TriColorStruct>();
	Object T0 = R.New<int>();
	Object T1 = R.New<int>();
	Object T2 = R.New<int>();
	tcs->contained->Append(T0);
	tcs->contained->Append(T1);
	tcs->contained->Append(T2);
	root.SetChild("tcs", tcs);

	R.TriColor();
	KAI_TEST_TRUE(root.Exists());
	KAI_TEST_TRUE(tcs.Exists());
	KAI_TEST_TRUE(tcs->contained.Exists());
	if (tcs->contained.Exists())
	{
		KAI_TEST_EQUIV(tcs->contained->Size(), 3);
	}
	
	KAI_TEST_TRUE(T0.Exists());
	KAI_TEST_TRUE(T1.Exists());
	KAI_TEST_TRUE(T2.Exists());

	root.RemoveChild("tcs");

	TriColor(R);

	KAI_TEST_TRUE(root.Exists());
	KAI_TEST_FALSE(tcs.Exists());
	KAI_TEST_FALSE(T0.Exists());
	KAI_TEST_FALSE(T1.Exists());
	KAI_TEST_FALSE(T2.Exists());

	tcs = R.New<TriColorStruct>();
	T0 = R.New<int>();
	T1 = R.New<int>();
	T2 = R.New<int>();
	tcs->contained->Append(T0);
	tcs->contained->Append(T1);
	tcs->contained->Append(T2);
	root.SetChild("tcs", tcs);

	R.TriColor();
	KAI_TEST_TRUE(root.Exists());
	KAI_TEST_TRUE(tcs.Exists());
	KAI_TEST_TRUE(tcs->contained.Exists());
	if (tcs->contained.Exists())
	{
		KAI_TEST_EQUIV(tcs->contained->Size(), 3);
	}
	KAI_TEST_TRUE(T0.Exists());
	KAI_TEST_TRUE(T1.Exists());
	KAI_TEST_TRUE(T2.Exists());

	T2.Delete();
	R.TriColor();
	DotGraph(root, "\\TestTriColor1.graph");

	KAI_TEST_TRUE(root.Exists());
	KAI_TEST_TRUE(tcs.Exists());
	KAI_TEST_TRUE(tcs->contained.Exists());
	if (tcs->contained.Exists())
	{
		KAI_TEST_EQUIV(tcs->contained->Size(), 2);
	}
	KAI_TEST_TRUE(T0.Exists());
	KAI_TEST_TRUE(T1.Exists());
	KAI_TEST_TRUE(!T2.Exists());

	M = R.New<int>();
	tcs.SetChild("M", M);
	R.TriColor();
	KAI_TEST_TRUE(root.Exists());
	KAI_TEST_TRUE(tcs.Exists());
	KAI_TEST_TRUE(tcs->contained.Exists());
	if (tcs->contained.Exists())
	{
		KAI_TEST_EQUIV(tcs->contained->Size(), 2);
	}
	KAI_TEST_TRUE(M.Exists());
	KAI_TEST_TRUE(T0.Exists());
	KAI_TEST_TRUE(T1.Exists());
	KAI_TEST_FALSE(T2.Exists());

	M.Delete();

	R.TriColor();
	KAI_TEST_TRUE(root.Exists());
	KAI_TEST_TRUE(tcs.Exists());
	KAI_TEST_FALSE(M.Exists());
	KAI_TEST_TRUE(T0.Exists());
	KAI_TEST_TRUE(T1.Exists());
	KAI_TEST_FALSE(T2.Exists());

	Object other = R.New<int>();
	tcs.SetPropertyObject("other", other);
	KAI_TEST_TRUE(other.Exists());
	R.TriColor();
	KAI_TEST_TRUE(root.Exists());
	KAI_TEST_TRUE(tcs.Exists());
	KAI_TEST_FALSE(M.Exists());
	KAI_TEST_TRUE(other.Exists());
	KAI_TEST_TRUE(T0.Exists());
	KAI_TEST_TRUE(T1.Exists());
	KAI_TEST_FALSE(T2.Exists());

	tcs.Delete();
	R.TriColor();
	KAI_TEST_FALSE(other.Exists());
	KAI_TEST_FALSE(tcs.Exists());
}

void TestTriColor::TestNested()
{
	Registry R;
	Tree tree;
	ClassBuilder<TriColorStruct>(R, "TriColorStruct")
		.Methods
		.Properties
			("contained", &TriColorStruct::contained, "", CreateParams::Create)
			("other", &TriColorStruct::other)
		;
	R.AddClass<void>();
	R.AddClass<Object>();
	R.AddClass<int>();
	List::Register(R);

	Object root = R.New<void>();
	tree.SetRoot(root);
	R.SetTree(tree);

	Pointer<TriColorStruct> tcs = R.New<TriColorStruct>();
	Pointer<List> list = R.New<List>();
	tcs->other = list;
	Object T0 = R.New<int>();
	Object T1 = R.New<int>();
	list->Append(T0);
	list->Append(T1);

	root.SetChild("tcs", tcs);
	R.GarbageCollect(root);
	KAI_TEST_TRUE(tcs.Exists());
	KAI_TEST_TRUE(list.Exists());
	KAI_TEST_TRUE(T0.Exists());
	KAI_TEST_TRUE(T1.Exists());

	list->Erase(T1);
	R.GarbageCollect(root);
	KAI_TEST_TRUE(tcs.Exists());
	KAI_TEST_TRUE(list.Exists());
	KAI_TEST_TRUE(T0.Exists());
	KAI_TEST_TRUE(!T1.Exists());

	T1 = R.New<int>();
	root.SetChild("T1", T1);
	if (list.Exists())
		list->Append(T1);
	R.GarbageCollect(root);
	KAI_TEST_TRUE(tcs.Exists());
	KAI_TEST_TRUE(list.Exists());
	KAI_TEST_TRUE(T0.Exists());
	KAI_TEST_TRUE(T1.Exists());

	if (list.Exists())
		list->Erase(T1);
	R.GarbageCollect(root);
	KAI_TEST_TRUE(T1.Exists());
	KAI_TEST_TRUE(list.Exists());

	root.RemoveChild("T1");
	R.GarbageCollect(root);
	KAI_TEST_TRUE(!T1.Exists());

	Pointer<List> list2 = R.New<List>();
	T1 = R.New<void>();
	list2->Append(T1);
	if (list.Exists())
		list->Append(T1);

	KAI_TEST_TRUE(T1.Exists());
	R.GarbageCollect();
	KAI_TEST_TRUE(T1.Exists());
	KAI_TEST_TRUE(list.Exists());

	if (list.Exists())
		list->Erase(T1);
	R.GarbageCollect();
	KAI_TEST_TRUE(!list2.Exists());
	KAI_TEST_TRUE(!T1.Exists());
}

void TestTriColor::TestMultipleReferences()
{
	Registry R;
	Tree tree;
	ClassBuilder<TriColorStruct>(R, "TriColorStruct")
		.Methods
		.Properties
			("contained", &TriColorStruct::contained, "", CreateParams::Create)
			("other", &TriColorStruct::other)
		;
	R.AddClass<void>();
	R.AddClass<Object>();
	R.AddClass<int>();
	List::Register(R);

	Object root = R.New<void>();
	tree.SetRoot(root);
	R.SetTree(tree);
	R.TriColor();

	Pointer<TriColorStruct> tcs1 = R.New<TriColorStruct>();
	Pointer<TriColorStruct> tcs2 = R.New<TriColorStruct>();
	
	tcs1->other = tcs2;
	Object Q = R.New<int>();
	tcs2->other = Q;
	tcs2->contained->Append(Q);
	Object Q2 = R.New<int>();
	Q.SetChild("Q2", Q2);

	root.SetChild("tcs1", tcs1);
	R.TriColor();

	KAI_TEST_TRUE(tcs1.Exists());
	KAI_TEST_TRUE(tcs2.Exists());
	KAI_TEST_TRUE(tcs1->contained.Exists());
	KAI_TEST_TRUE(tcs2->contained.Exists());
	KAI_TEST_TRUE(Q.Exists());
	KAI_TEST_TRUE(Q2.Exists());

	tcs2->other = Object();
	R.TriColor();
	KAI_TEST_TRUE(tcs1.Exists());
	KAI_TEST_TRUE(tcs2.Exists());
	KAI_TEST_TRUE(tcs1->contained.Exists());
	KAI_TEST_TRUE(tcs2->contained.Exists());
	KAI_TEST_TRUE(!tcs2->other.Exists());
	KAI_TEST_TRUE(Q.Exists());
	KAI_TEST_TRUE(Q2.Exists());

	tcs2->contained->Erase(Q);
	R.TriColor();
	KAI_TEST_TRUE(tcs1.Exists());
	KAI_TEST_TRUE(tcs2.Exists());
	KAI_TEST_TRUE(tcs1->contained.Exists());
	KAI_TEST_TRUE(tcs2->contained.Exists());
	KAI_TEST_TRUE(!Q.Exists());
	KAI_TEST_TRUE(!Q2.Exists());

	tcs2.Delete();
	R.TriColor();
	KAI_TEST_TRUE(tcs1.Exists());
	KAI_TEST_TRUE(!tcs2.Exists());
	KAI_TEST_TRUE(!tcs1->other.Exists());


	Object A = R.New<int>();
	Object B = R.New<int>();
	Object C = R.New<int>();

	root.SetChild("A", A);
	A.SetChild("B", B);
	B.SetChild("C", C);
	R.TriColor();
	KAI_TEST_TRUE(A);
	KAI_TEST_TRUE(B);
	KAI_TEST_TRUE(C);

	B.Delete();
	R.TriColor();
	KAI_TEST_TRUE(A);
	KAI_TEST_TRUE(!B.Exists());
	KAI_TEST_TRUE(!C.Exists());

	A.Delete();
	B.Delete();
	C.Delete();
	R.TriColor();
	KAI_TEST_TRUE(!A);
	KAI_TEST_TRUE(!B);
	KAI_TEST_TRUE(!C);
	KAI_TEST_TRUE(!root.Has("A"));

	A = R.New<int>();
	B = R.New<int>();
	C = R.New<int>();

	root.SetChild("A", A);
	root.SetChild("C", C);
	A.SetChild("B", B);
	B.SetChild("C", C);

	A.Delete();
	R.TriColor();

	KAI_TEST_TRUE(!A);
	KAI_TEST_TRUE(!B);
	KAI_TEST_TRUE(C);
}

void TestTriColor::TestOverride()
{
	Registry R;
	Tree tree;
	ClassBuilder<TriColorStruct>(R, "TriColorStruct")
		.Methods
		.Properties
			("contained", &TriColorStruct::contained)
			("other", &TriColorStruct::other)
		;
	R.AddClass<void>();
	R.AddClass<Object>();
	R.AddClass<int>();
	List::Register(R);

	Object root = R.New<void>();
	tree.SetRoot(root);
	R.SetTree(tree);
	R.TriColor();

	Object A = R.New<int>();
	Object B = R.New<int>();
	A.SetChild("B", B);
	root.SetChild("A", A);

	R.TriColor();
	KAI_TEST_TRUE(A);
	KAI_TEST_TRUE(B);

	Object B2 = R.New<int>();
	A.SetChild("B", B2);
	R.TriColor();
	KAI_TEST_TRUE(A);
	KAI_TEST_TRUE(B2);
	KAI_TEST_TRUE(!B);

	// test properties
	Pointer<TriColorStruct> tcs = R.New<TriColorStruct>();
	Object N = R.New<int>();

	tcs->other = N;
	root.SetChild("tcs", tcs);

	R.TriColor();
	KAI_TEST_TRUE(tcs);
	KAI_TEST_TRUE(tcs->other);

	Object N2 = R.New<int>();
	tcs->other = N2;
	R.TriColor();
	KAI_TEST_TRUE(tcs);
	KAI_TEST_TRUE(tcs->other);
	KAI_TEST_FALSE(N);
	KAI_TEST_TRUE(N2);
}

void TestTriColor::TestContainers()
{
	Registry R;
	Tree tree;
	ClassBuilder<TriColorStruct>(R, "TriColorStruct")
		.Methods
		.Properties
			("contained", &TriColorStruct::contained)
			("other", &TriColorStruct::other)
		;
	R.AddClass<void>();
	R.AddClass<Object>();
	R.AddClass<int>();
	List::Register(R);
	
	Pointer<List> list = R.New<List>();
	Pointer<int> n0 = R.New(123);
	Pointer<int> n1 = R.New(456);
	list->Append(n0);
	list->Append(n1);
	R.AddRoot(list);

	R.TriColor();
	KAI_TEST_TRUE(list);
	KAI_TEST_TRUE(n0);
	KAI_TEST_TRUE(n1);
	KAI_TEST_TRUE(list->Contains(n0));
	KAI_TEST_TRUE(list->Contains(n1));
	KAI_TEST_EQUIV(list->Size(), 2);

	n0.Delete();
	R.TriColor();
	KAI_TEST_TRUE(list);
	KAI_TEST_FALSE(n0);
	KAI_TEST_TRUE(n1);
	KAI_TEST_FALSE(list->Contains(n0));
	KAI_TEST_TRUE(list->Contains(n1));
	KAI_TEST_EQUIV(list->Size(), 1);

	//foreach (Pointer<const int> num, *list)
	//{
	//	int N = *num;
	//}
}

void TestTriColor::Pre()
{
	//registry = new Registry();
	//tree = new Tree();
	//Registry &R = *registry;

	//ClassBuilder<TriColorStruct>(R, "TriColorStruct")
	//	.Methods
	//	.Properties
	//		("contained", &TriColorStruct::contained)
	//		("other", &TriColorStruct::other)
	//	;
	//R.AddClass<void>();
	//R.AddClass<Object>();
	//R.AddClass<int>();
	//List::Register(R);

	//Object root = R.New<void>();
	//tree->SetRoot(root);
	//R.SetTree(tree);
}

void TestTriColor::Post()
{
	//delete registry;
	//delete tree;1
}

void TestTriColor::TriColor(Registry &R)
{
	R.TriColor();
}

KAI_TESTS_END

#endif // KAI_UNIT_TESTS

//EOF
