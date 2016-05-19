
#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include "KAI/Tests/TestBinaryStream.h"

KAI_TESTS_BEGIN

void TestBinaryStream::TestBuiltins()
{
	BinaryStream S;

	S << 42;
	KAI_TEST_TRUE(S.Size() == sizeof(int));
	KAI_TEST_TRUE(S.CanRead(sizeof(int)));
	int N;
	S >> N;
	KAI_TEST_EQUIV(N, 42);
	KAI_TEST_FALSE(S.CanRead(1));

	S.Clear();
	KAI_TEST_TRUE(S.Size() == 0);
	KAI_TEST_FALSE(S.CanRead(1));

	S << String("Hello, world");
	String T;
	S >> T;
	KAI_TEST_FALSE(S.CanRead(1));
	KAI_TEST_EQUIV(T, "Hello, world");
}

void TestBinaryStream::TestObject()
{
	Registry R;
	R.AddClass<int>("int");

	BinaryStream S;
	S.SetRegistry(&R);

	Pointer<int> N = R.New(42);
	S << N;
	Object Q;
	S >> Q;
	KAI_TEST_FALSE(S.CanRead(1));
	KAI_TEST_EQUIV(Q.GetTypeNumber(), Type::Traits<int>::Number);
	KAI_TEST_EQUIV(ConstDeref<int>(Q), 42);

	S.Clear();
	KAI_TEST_TRUE(S.Empty());

	N.Set("child0", R.New(123));
	S << N;
	Object M;
	S >> M;
	KAI_TEST_FALSE(S.CanRead(1));
	KAI_TEST_EQUIV(ConstDeref<int>(M), 42);
	KAI_TEST_TRUE(M.Has("child0"));
	KAI_TEST_EQUIV(ConstDeref<int>(M.Get("child0")), 123);
}

void TestBinaryStream::TestArray()
{
	Registry R;
	R.AddClass<Array>("Array");
	R.AddClass<int>("int");

	Pointer<Array> A = R.New<Array>();
	A->Append(R.New(0));
	A->Append(R.New(1));
	A->Append(R.New(2));
	A->Append(R.New(3));

	BinaryStream S;
	S.SetRegistry(&R);

	S << A;
	Object Q;
	S >> Q;

	KAI_TEST_FALSE(S.CanRead(1));
	KAI_TEST_EQUIV(Q.GetTypeNumber(), Type::Traits<Array>::Number);
	const Array &B = ConstDeref<Array>(Q);
	KAI_TEST_EQUIV(B.Size(), 4);
	KAI_TEST_EQUIV(ConstDeref<int>(B.At(0)), 0);
	KAI_TEST_EQUIV(ConstDeref<int>(B.At(1)), 1);
	KAI_TEST_EQUIV(ConstDeref<int>(B.At(2)), 2);
	KAI_TEST_EQUIV(ConstDeref<int>(B.At(3)), 3);
}

struct TestPropertiesStruct
{
	int num;
	String str;
	Object foo;
};

KAI_TESTS_TYPE_TRAITS(TestPropertiesStruct, 555, Type::Properties::None);

void TestBinaryStream::TestProperties()
{
	Registry R;
	R.AddClass<void>();
	R.AddClass<int>();
	R.AddClass<String>();
	ClassBuilder<TestPropertiesStruct>(R, "TestPropertiesStruct")
		.Methods
		.Properties
			("num", &TestPropertiesStruct::num)
			("str", &TestPropertiesStruct::str)
			("foo", &TestPropertiesStruct::foo)
		;
	Pointer<TestPropertiesStruct> tps = R.New<TestPropertiesStruct>();
	tps->num = 42;
	tps->str = "hello";
	tps->foo = R.New<TestPropertiesStruct>();
	KAI_TRACE_1(tps);

	BinaryStream stream(R);
	stream << tps;
	KAI_TRACE_1(stream);
	Object result;
	stream >> result;
	KAI_TRACE_1(result);

	KAI_TEST_TRUE(result.Exists());
	KAI_TEST_TRUE(result.IsType<TestPropertiesStruct>());
	KAI_TEST_EQUIV(result.GetValue<int>("num"), 42);
	KAI_TEST_EQUIV(result.GetValue<String>("str"), "hello");
	KAI_TEST_TRUE(result.Get("foo").IsType<TestPropertiesStruct>());
}

void TestBinaryStream::TestList()
{
	Registry R;
	List::Register(R);
	R.AddClass<void>("void");
	R.AddClass<int>("int");
	R.AddClass<String>("String");

	Pointer<List> list = R.New<List>();
	list->Append(R.New(123));
	list->Append(R.New(456));
	list->Append(R.New(String("Hello")));
	BinaryStream stream(R);
	stream << list;
	Object result;
	stream >> result;

	KAI_TRACE_1(list);
	KAI_TRACE_1(stream);
	KAI_TRACE_1(result);

	KAI_TEST_TRUE(result.Exists());
	KAI_TEST_TRUE(result.IsType<List>());
	KAI_TEST_EQUIV(ConstDeref<List>(result).Size(), 3);
	KAI_TEST_EQUIV(result, list);

	Pointer<List> result_list = result;
	KAI_TEST_EQUIV(ConstDeref<String>(result_list->Back()), "Hello");
	result_list->PopBack();
	KAI_TEST_EQUIV(ConstDeref<int>(result_list->Back()), 456);
	result_list->PopBack();
	KAI_TEST_EQUIV(ConstDeref<int>(result_list->Back()), 123);
	result_list->PopBack();
	KAI_TEST_EQUIV(ConstDeref<List>(result_list).Size(), 0);
}

void TestBinaryStream::TestMap()
{
	KAI_TEST_NOT_IMPLEMENTED();
}

void TestBinaryStream::TestSet()
{
	KAI_TEST_NOT_IMPLEMENTED();
}

void TestBinaryStream::TestStreams()
{
	KAI_TEST_NOT_IMPLEMENTED();
}

KAI_TESTS_END

#endif // KAI_UNIT_TESTS

//EOF
