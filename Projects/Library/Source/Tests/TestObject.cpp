
#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include "KAI/Tests/TestObject.h"

#	pragma warning(disable:4101)
#	pragma warning(disable:4189)

KAI_TESTS_BEGIN

void TestObject::TestDictionary()
{
	// all objects are created from a registry. many registries can exist in the same process;
	// indeed, registries can and should exist on a stack
	Registry R;

	// add classes to the registry. note that we use C++ types explicitly, including void
	R.AddClass<void>();
	R.AddClass<int>();
	R.AddClass<String>();

	// make a new instance of type `void`: this has no value but can serve as a container
	Object object = R.New<void>();

	// set a child of `object` called 'num' to be an int with value 42
	object.SetValue("num", 42);

	// test the the obect does indeed has a property called 'num', that it is an int, and
	// that its value is 42
	KAI_TEST_TRUE(object.Has("num"));
	KAI_TEST_TRUE(object.Get("num").IsType<int>());
	KAI_TEST_EQUIV(object.GetValue<int>("num"), 42);

	// set another child with name `str` and a String value of "hello"
	object.SetValue("str", String("hello"));

	KAI_TEST_TRUE(object.Has("str"));
	KAI_TEST_TRUE(object.Get("str").IsType<String>());
	KAI_TEST_EQUIV(object.GetValue<String>("str"), "hello");

	// make a new object of type String. note that we type it to be a string
	Pointer<String> sub_object = R.New(String("world"));

	// Pointer<T> looks a lot like a T *
	*sub_object = "foo";

	// but all Pointer<T>'s are derived from Object and as such are implicitly convertible
	object.Set("sub_object", sub_object);
	KAI_TEST_EQUIV(object.GetValue<String>("sub_object"), "foo");

	// use syntactic sugar to set and get properties: object[label] = value is an abbreviation of object.SetValue(label, value)
	object["some_number"] = 123;
	object["some_string"] = String("some string or other");

	// test that we can also get the values safely
	KAI_TEST_EQUIV(ConstDeref<int>(object["some_number"]), 123);
	KAI_TEST_EQUIV(ConstDeref<String>(object["some_string"]), "some string or other");

	KAI_TEST_EQUIV(object.GetValue<int>("some_number"), 123);
	KAI_TEST_EQUIV(object.GetValue<String>("some_string"), "some string or other");

	// trace out what an object looks like, as a trace, as a string and as an XML string
	KAI_TRACE_1(object);
	KAI_TRACE_1(object.ToString());
	KAI_TRACE_1(object.ToXmlString());

	sub_object["another_string"] = String("this is another string. woot");
	sub_object["number"] = 1234;

	Object sub_sub_object = R.New(String("spam"));
	sub_object["spam"] = sub_sub_object;
	sub_sub_object["grok"] = 456;
	KAI_TRACE_1(object.ToXmlString());

	// all objects can be inserted/extracted to/from binary packets
	BinaryStream stream(R);
	stream << object;
	Object result;
	stream >> result;
	KAI_TRACE_1(stream);
	KAI_TRACE_1(result.ToXmlString());

	// test that the result after extraction from the binary stream is indeed
	// the same as the original object
	KAI_TEST_EQUIV(result, object);

	// change it, and test that they are now different
	result["str"] = String("not the same");
	KAI_TEST_NOT_EQUIV(result, object);

	// set it back and test that they are now the same
	result["str"] = String("hello");
	KAI_TEST_EQUIV(result, object);

	// objects can also be tested for ordering (including all properties and sub-objects)
	KAI_TEST_TRUE(!(result < object));
	KAI_TEST_TRUE(!(object < result));

	// test that ordering is strict and weak
	result["num"] = 1;
	object["num"] = 3;
	KAI_TEST_TRUE(result < object);
	KAI_TEST_FALSE(object < result);
	KAI_TEST_FALSE(object == result);
	result["num"] = 3;
	object["num"] = 1;
	KAI_TEST_TRUE(object < result);
	KAI_TEST_FALSE(result < object);
	KAI_TEST_FALSE(object == result);
	result["num"] = 1;
	object["num"] = 1;
	KAI_TEST_TRUE(object == result);
	KAI_TEST_FALSE(result < object);
	KAI_TEST_FALSE(object < result);
}

void TestObject::TestConstruction()
{
	// central depot for types and instances
	Registry R;

	// add some types
	R.AddClass<int>();
	R.AddClass<String>();

	// create a const int using compile-time type info
	Pointer<const int> N = R.New<int>();

	// FAIL: 'const int' does not have traits; hosever the type could be added
	//Pointer<const int> N = R.New<const int>();

	// create an int using a type number
	N = R.NewFromTypeNumber(Type::Number::Signed32);

	// create an int using a string type id
	N = R.NewFromClassName("int");

	// create a mutable int
	Pointer<int> M = R.New<int>();

	Pointer<int> P;			// P is a pointer to a mutable int
	Pointer<const int> Q;	// Q is a pointer to a constant int

	// OK to assign a pointer to another pointer
	P = M;

	// OK to dereference and assign to a pointer
	*P = 42;

	// FAIL to assign a const pointer to a pointer
	// error C2248: 'pre::Pointer<T>::operator =' : cannot access private member declared in class 'pre::Pointer<T>'
	//P = Q;		
	
	// OK to assign a pointer to a const pointer
	Q = P;

	// OK to deref a const pointer
	int O = *Q;

	// ensure that M's referent is the same as P's
	KAI_TEST_EQUIV(*M, 42);

	Q = N;

	// FAIL to assign to a deferenced const pointer
	// error C3892: 'Q' : you cannot assign to a variable that is const
	//*Q = 42;

	Object K = Q;
	Object L = P;

	{
		Pointer<const int> Q;
		// FAIL at compile-time. Can't make a Pointer<T> from a Pointer<const T>
		//Pointer<int> P = Q;
	}

	// types are as strongly checked as possble at compile-time, and then again at runtime
	bool caught = false;
	try
	{
		Pointer<int> P = R.New<int>();
		Object Q = P;				// "upcast" to most generic type
		Pointer<String> F = Q;		// throws at runtime
	}
	catch (Exception::TypeMismatch &)
	{
		caught = true;
	}
	KAI_TEST_TRUE(caught);
	caught = false;

	// Constness is respected
	try
	{
		Pointer<String> P = R.New<String>();
		KAI_TEST_FALSE(P.IsConst());

		Pointer<const String> Q = P;		// OK to make const from non-const
		KAI_TEST_TRUE(Q.IsConst());

		// FAIL: compile-time fail
		//Pointer<String> K = Q;

		// OK: Object's are generic irrespective of type
		Object A = Q;

		// OK: Pointer<> cannot know that A actually indicates a const object
		Pointer<String> B = A;

		// FAIL: throws ConstError at runtime
		*B = "FAIL";
	}
	catch (Exception::ConstError &)
	{
		caught = true;
	}
	KAI_TEST_TRUE(caught);
	caught = false;

	// OK: compiles; but will throw ConstError at runtime
	try
	{
		Pointer<int> P = R.New<int>();
		*P = 42;				// OK
		Object J = P;			// generalise
		Deref<int>(J) = 42;		// OK

		Pointer<const int> Q = R.New<int>();
		Deref<int>(Object(Q)) = 64;	// FAIL? Unclear what should happen here...
	}
	catch (Exception::ConstError &)	
	{
		caught = true;
	}
	//assert(caught);

	// OK
	Deref<int>(P) = 43;

	// OK: Q is a ConstPointer
	int Y = ConstDeref<int>(Q);

	// OK: P is a Pointer
	int &Z = Deref<int>(P);
}

void TestObject::TestDirtyBit()
{
	Registry R;
	R.AddClass<int>("int");

	Pointer<int> N = R.New(42);

	KAI_TEST_TRUE(N.IsDirty());

	N.SetClean();
	KAI_TEST_TRUE(N.IsClean());

	int M = *N;
	KAI_TEST_TRUE(N.IsClean());

	*N = 12;
	KAI_TEST_TRUE(N.IsDirty());
}

void TestObject::TestAttributes()
{
}

KAI_TESTS_END

#endif

//EOF
