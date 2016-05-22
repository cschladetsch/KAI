

#include "KAI/Test/Base.h"
#include <iostream>
#ifdef KAI_UNIT_TESTS
#	include "KAI/Tests/TestRegistry.h"

KAI_BEGIN

struct TestPropertyMarkingStruct
{
	Pointer<int> N;
	Pointer<int> S;
};

KAI_TYPE_TRAITS(TestPropertyMarkingStruct, 999, Type::Properties::None);

KAI_END

KAI_TESTS_BEGIN

TestRegistry::TestRegistry()
{
	Builder(this, "TestRegistry")
		("TestConstruction", &TestRegistry::TestConstruction)
		("TestMarkAndSweep", &TestRegistry::TestMarkAndSweep)
		("TestMarkAndSweepContainers", &TestRegistry::TestMarkAndSweepContainers)
		("TestPropertyMarkingPersistence", &TestRegistry::TestPropertyMarkingPersistence)
		("TestPropertyMarkingRemoval", &TestRegistry::TestPropertyMarkingRemoval)
		("TestContainers", &TestRegistry::TestContainers)
		;
}

void TestRegistry::TestConstruction()
{
	Registry R;
	R.AddClass<int>();
	KAI_TEST_TRUE(R.GetClass<int>() != 0);
	KAI_TEST_EQUIV(R.GetClass("int"), R.GetClass(Type::Number::Signed32));

	Pointer<int> N;
	KAI_TEST_FALSE(N.Valid());
	KAI_TEST_FALSE(N.Exists());
	
	N = R.New<int>();
	
	KAI_TEST_TRUE(N.Valid());
	KAI_TEST_TRUE(N.Exists());
}

void TestRegistry::TestMarkAndSweep()
{
	Registry R;
	R.AddClass<void>();
	R.AddClass<int>();
	R.AddClass<float>();
	R.AddClass<String>();
	R.AddClass<Array>();

	// create a root object
	Object root = R.New<void>();
	KAI_TEST_EQUIV(R.GetInstances().size(), std::size_t(1));

	// create an unreferenced object
	Object dangling = R.New<int>();
	KAI_TEST_TRUE(dangling.Exists());
	KAI_TEST_EQUIV(R.GetInstances().size(), std::size_t(2));
	
	// delete unreferenced objects
	R.GarbageCollect(root);

	// ensure dangling objects are removed after mark and sweep
	KAI_TEST_FALSE(dangling.Exists());
	KAI_TEST_EQUIV(R.GetInstances().size(), std::size_t(1));
}

void TestRegistry::TestContainers()
{
	Registry R;
	R.AddClass<void>();
	R.AddClass<int>();
	R.AddClass<String>();
	R.AddClass<Array>();
	Object root = R.New<void>();

	// make some new objects
	Pointer<String> S = R.New<String>();
	Pointer<int> N = R.New<int>();
	Pointer<Array> A = R.New<Array>();
	A->Append(S);
	A->Append(N);
	root.Set("A", A);
	
	Pointer<int> M = R.New<int>();

	KAI_TEST_TRUE(R.GetInstances().size() == std::size_t(5));
	R.GarbageCollect(root);
	KAI_TEST_TRUE(R.GetInstances().size() == std::size_t(4));
	KAI_TEST_TRUE(S.Exists());
	KAI_TEST_TRUE(N.Exists());
	KAI_TEST_TRUE(A.Exists());
	KAI_TEST_TRUE(root.Exists());
	KAI_TEST_FALSE(M.Exists());

	KAI_TEST_TRUE(S.Exists());
	A->Erase(S);
	KAI_TEST_TRUE(S.Exists());
	R.GarbageCollect(root);
	//KAI_TEST_FALSE(S.Exists());

	// test automatic marking of properties
	ClassBuilder<TestPropertyMarkingStruct>(R, "TestPropertyMarking")
		.Methods
		.Properties
		("N", &TestPropertyMarkingStruct::N)
		("S", &TestPropertyMarkingStruct::S)
		;

	int num_instances = (int)R.GetInstances().size();
	Pointer<TestPropertyMarkingStruct> K = R.New<TestPropertyMarkingStruct>();
	R.GarbageCollect(root);
	KAI_TEST_FALSE(K.Exists());
	KAI_TEST_EQUIV((int)R.GetInstances().size(), num_instances);
}

void TestRegistry::TestMarkAndSweepContainers()
{
	Registry R;
	R.AddClass<void>("void");
	R.AddClass<int>("int");
	Array::Register(R);

	Pointer<Array> array = R.New<Array>();
	Object object = R.New<void>();
	object.Set("child", R.New<int>(42));
	array->Append(object);
	
	R.GarbageCollect(array);

	KAI_TEST_TRUE(array.Exists());
	KAI_TEST_TRUE(object.Exists());
	KAI_TEST_TRUE(object.Has("child"));
	KAI_TEST_TRUE(object.Get("child").Exists());
}

void TestRegistry::TestPropertyMarkingPersistence()
{
	Registry R;
	R.AddClass<void>("void");
	R.AddClass<int>("int");
	ClassBuilder<TestPropertyMarkingStruct>(R, "TestPropertyMarking")
		.Methods
		.Properties
		("N", &TestPropertyMarkingStruct::N)
		("S", &TestPropertyMarkingStruct::S)
		;

	// create root object
	Object root = R.New<void>();

	// now, make a new one and set its properties
	Pointer<TestPropertyMarkingStruct> K = R.New<TestPropertyMarkingStruct>();
	K->N = R.New(42);
	K->S = R.New(123);

	// add it to the root object
	root.Set("K", K);

	// destroy everything that cannot be reached from root
	R.GarbageCollect(root);

	// ensure K and its properties still exist
	KAI_TEST_TRUE(K.Exists());
	KAI_TEST_TRUE(K->N.Exists());
	KAI_TEST_TRUE(K->S.Exists());

	// test nested children. children of properties should be marked
	// when the property is marked
	// add some children to properties of K
	Object M2 = R.New(84);
	Object T2 = R.New(22);
	K->N.Set("M2", M2);
	K->S.Set("T2", T2);

	// delete everything not reachable from root
	R.GarbageCollect(root);

	// ensure everything still exists
	KAI_TEST_TRUE(K.Exists());
	KAI_TEST_TRUE(K->N.Exists());
	KAI_TEST_TRUE(K->S.Exists());
	KAI_TEST_TRUE(M2.Exists());
	KAI_TEST_TRUE(T2.Exists());
	KAI_TEST_TRUE(K->N.Get("M2").Exists());
	KAI_TEST_TRUE(K->S.Get("T2").Exists());

	// delete the parent, ensure the properties are deleted as well
	K.Delete();
	R.GarbageCollect(root);
	KAI_TEST_FALSE(K.Exists());
	KAI_TEST_FALSE(T2.Exists());
	KAI_TEST_FALSE(M2.Exists());
}

void TestRegistry::TestPropertyMarkingRemoval()
{
	Registry R;
	R.AddClass<void>("void");
	R.AddClass<int>("int");
	ClassBuilder<TestPropertyMarkingStruct>(R, "TestPropertyMarking")
		.Methods
		.Properties
		("N", &TestPropertyMarkingStruct::N)
		("S", &TestPropertyMarkingStruct::S)
		;

	Object root = R.New<void>();

	// make a new object set its properties
	Pointer<TestPropertyMarkingStruct> K = R.New<TestPropertyMarkingStruct>();
	K->N = R.New(42);
	K->S = R.New(123);

	// add it to the root object
	root.Set("K", K);

	Object M2 = R.New(84);
	Object T2 = R.New(22);

	// remove a child from a property
	KAI_TEST_TRUE(M2.Exists());
	KAI_TEST_FALSE(M2.OnDeathRow());
	M2.Delete();
	//KAI not work for tricolor KAI_TEST_TRUE(M2.OnDeathRow());
	KAI_TEST_TRUE(M2.Exists());

	// delete everything not reachable from root
	M2 = R.New(84);
	T2 = R.New(22);
	K->N.Set("M2", M2);
	K->S.Set("T2", T2);
	R.GarbageCollect(root);
	KAI_TEST_TRUE(K.Exists());
	KAI_TEST_TRUE(K->N.Exists());
	KAI_TEST_TRUE(K->S.Exists());
	KAI_TEST_TRUE(K->N.Get("M2").Exists());
	KAI_TEST_TRUE(K->S.Get("T2").Exists());

	// test deleting a child of a property
	M2.Delete();
	R.GarbageCollect(root);
	KAI_TEST_TRUE(K.Exists());
	KAI_TEST_TRUE(K->N.Exists());
	KAI_TEST_TRUE(K->S.Exists());
	KAI_TEST_FALSE(K->N.Has("M2"));
	KAI_TEST_FALSE(M2.Exists());
	KAI_TEST_TRUE(K->S.Get("T2").Exists());

	// test that deleting the parent of properties destroyes all properties and child objects
	K.Delete();
	R.GarbageCollect(root);
	KAI_TEST_FALSE(K.Exists());
	KAI_TEST_FALSE(M2.Exists());
	KAI_TEST_FALSE(T2.Exists());

	// test that it's ok to delete a non-existing object
	T2.Delete();
	R.GarbageCollect(root);
	KAI_TEST_FALSE(T2.Exists());
	KAI_TEST_FALSE(M2.Exists());
}

KAI_TESTS_END

#endif

//EOF
