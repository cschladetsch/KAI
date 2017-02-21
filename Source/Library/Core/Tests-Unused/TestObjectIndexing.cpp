
#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include "KAI/Tests/TestObjectIndexing.h"

KAI_BEGIN

struct TestObjectIndexingTest
{
	int num;
	Pointer<int> ptr;
};

KAI_TYPE_TRAITS(TestObjectIndexingTest, 666, Properties::None);

KAI_END

KAI_TESTS_BEGIN

void TestObjectIndexing::Test()
{
	Registry R;
	R.AddClass<void>();
	R.AddClass<int>();
	R.AddClass<float>();
	R.AddClass<String>();

	Pointer<void> N = R.New<void>();

	N["int"] = 32;
	N["float"] = 2.13f;
	N["string"] = String("hello");

	KAI_TEST_TRUE(N.HasChild("int"));
	KAI_TEST_TRUE(N.HasChild("float"));
	KAI_TEST_TRUE(N.HasChild("string"));

	KAI_TEST_EQUIV(ConstDeref<int>(N["int"]), 32);
	KAI_TEST_EQUIV(ConstDeref<float>(N["float"]), 2.13f);
	KAI_TEST_EQUIV(ConstDeref<String>(N["string"]), "hello");

	ClassBuilder<TestObjectIndexingTest>(R, "TestObjectIndexingTest")
		.Methods
		.Properties
			("num", &TestObjectIndexingTest::num)
			("ptr", &TestObjectIndexingTest::ptr)
		;

	Pointer<TestObjectIndexingTest> toit = R.New<TestObjectIndexingTest>();
	toit["ptr"] = 32;
	toit["num"] = 45;

	KAI_TEST_TRUE(toit.Get("num").IsType<int>());
	KAI_TEST_TRUE(toit.Get("ptr").IsType<int>());
	KAI_TEST_EQUIV(toit.GetValue<int>("ptr"), 32);
	KAI_TEST_EQUIV(toit.GetValue<int>("num"), 45);

	Pointer<int> ptr = toit["ptr"];
	Pointer<int> num = toit["num"];

	KAI_TEST_TRUE(ptr);
	KAI_TEST_TRUE(num);
	KAI_TEST_EQUIV(*ptr, 32);
	KAI_TEST_EQUIV(*num, 45);

	num = R.New(123);
	toit["num"] = num;
	KAI_TEST_EQUIV(Deref<int>(toit["num"]), 123);

	bool caught = false;
	KAI_TRY
	{
		toit["num"] = String("fail");
	}
	KAI_CATCH(Exception::TypeMismatch, E)
	{
		KAI_UNUSED(E);
		caught = true;
	}
	KAI_TEST_TRUE(caught);
}

void TestObjectIndexing::Pre()
{
}

void TestObjectIndexing::Post()
{
}

KAI_TESTS_END

#endif // KAI_UNIT_TESTS

//EOF
