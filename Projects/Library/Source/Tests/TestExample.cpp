
#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include "KAI/Test/Base.h"
#	include "KAI/Tests/TestExample.h"
#	include "KAI/BuiltinTypes/All.h"
#	include "KAI/Bin/Base.h"
#	include "KAI/Tree.h"
#	include "KAI/Console.h"

KAI_TESTS_BEGIN

/// this is a C++ struct that we will reflect to access at runtime via scripting
// note that we do not have to derive from any magic base classes
// note as well that there is no need for magic macros in the class definition
// this also means that we can reflect any C++ class, even if we do not
// have access to its source code. no recompile is needed.
class MyStruct
{
public:
	// this is accessible to the runtime after registration below
	int num;
	String string;

	// the Pointer<> wrapper makes this field a KAI object, which means it will
	// be subject to garbage collection
	Pointer<List> list;

	int Method0() 
	{ 
		KAI_TRACE_0();
		return 42; 
	}

	String Method1(int n, String str) const
	{
		KAI_TRACE_1(str);
		String result;
		while (n-- > 0) 
			result += str;
		return result; 
	}

	void Method2()
	{
	}

	// every class that is to be reflected must be registered.
	// by convention, this is done in a static function called Register
	static void Register(Registry &);
};

// this is just stuff for the unit-testing suite. ignore.
KAI_TESTS_TYPE_TRAITS(MyStruct, 555, Type::Properties::None);

// register MyStruct with a registry. it couldn't be easier.
void MyStruct::Register(Registry &registry)
{
	ClassBuilder<MyStruct>(registry, "MyStruct")
		.Methods
			("Method0", &MyStruct::Method0, "A method")
			("Method1", &MyStruct::Method1, "Another method")
			("Method2", &MyStruct::Method2, "Another method")
		.Properties
			("num", &MyStruct::num, "a number")
			("string", &MyStruct::string, "a string")
			("list", &MyStruct::list, "a list")
		;
}

// just making some functions that we will also add to the runtime soon
void Function_A()
{
	KAI_TRACE_0();
}

void Function_B(int n)
{
	KAI_TRACE_1(n);
}

String Function_C(int n, float f, String p)
{
	KAI_TRACE_3(n,f,p);
	return p + "foo";
}

Object Function_D(Object object)
{
	KAI_TRACE_1(object);
	return object["num"];
}

// boilerplate for testing
void TestExample::Pre()
{
}

void TestExample::Post()
{
}

// the actual test
void TestExample::TestStruct()
{
	// we can give the KAI runtime a custom allocator to use. we will just use
	// the standard one which uses malloc and free
	Memory::StandardAllocator alloc;
	Console console(&alloc);

	// a registry is a factory for classes and instances
	Registry &reg = console.GetRegistry();

	// tell the registry about our structure
	MyStruct::Register(reg);

	// make a new instance. note that we use a registry to make a new instance.
	// after that, we can use that instance to make other instances, which will just
	// use the registry that created it...
	Pointer<MyStruct> mystruct = reg.New<MyStruct>();

	KAI_TEST_TRUE(mystruct.Exists());
	KAI_TEST_EQUIV(mystruct.GetTypeNumber(), Type::Traits<MyStruct>::Number);

	// can produce an XML representation
	KAI_TRACE_1(mystruct.ToXmlString());

	// objects an be inserted into string streams
	StringStream stream;
	stream << mystruct;
	KAI_TRACE_1(stream.ToString()); 

	// objects can also be inserted and extracted to/from binary streams
	// this is useful for networking and persistence.
	BinaryStream binary_stream(reg);
	binary_stream << mystruct;
	KAI_TRACE_1(binary_stream);

	// accessing fields and methods uses pointer semantics
	mystruct->num = 345;
	mystruct->string = "hello world";
	mystruct->list = reg.New<List>();
	mystruct->list->Append(reg.New(42));
	mystruct->list->Append(reg.New(123));
	mystruct->list->Append(reg.New("foobar"));

	// see what it looks like
	KAI_TRACE_1(mystruct.ToXmlString());

	// clear the stream, then see what mystruct looks like as a string
	stream.Clear();
	stream << mystruct;
	KAI_TRACE_1(stream.ToString());

	// objects are stored in a hierarchy; get the root of the hierarchy
	// add a path to the root of hte tree, add mystruct as a child of the root
	Object root = console.GetRoot();
	console.GetTree().AddSearchPath(Pathname("/"));
	root["mystruct"] = mystruct;

	// functions and methods get their arguments from and return results to a stack
	Pointer<Stack> stack;

	// add general functions to the root of the tree
	AddFunction(root, Function_A, "FunctionA");
	AddFunction(root, Function_B, "FunctionB");
	AddFunction(root, Function_C, "FunctionC");
	AddFunction(root, Function_D, "FunctionD");

	// invoke the functions; take copies of the resultant stacks after each function completes
	Pointer<Stack> stack_A = console.Execute("FunctionA();").Clone();
	Pointer<Stack> stack_B = console.Execute("Function_B(42)").Clone();
	Pointer<Stack> stack_C = console.Execute("FunctionC(123, 3.0, \"bar\");").Clone();
	Pointer<Stack> stack_D = console.Execute("FunctionD(mystruct)").Clone();

	// show results and test
	KAI_TRACE_1(stack_A);
	KAI_TRACE_1(stack_B);
	KAI_TRACE_1(stack_C);
	KAI_TRACE_1(stack_D);
	KAI_TEST_TRUE(stack_A->Empty());
	KAI_TEST_TRUE(stack_B->Empty());
	KAI_TEST_EQUIV(ConstDeref<String>(stack_C->Pop()), "barfoo");
	KAI_TEST_EQUIV(ConstDeref<int>(stack_D->Pop()), 345);

	// test calling methods
	stack = console.Execute("mystruct.'Method0()");
	KAI_TEST_EQUIV(Deref<int>(stack->Pop()), 42);
	
	stack = console.Execute("mystruct.'Method1(2, \"foo\");");
	KAI_TEST_EQUIV(Deref<String>(stack->Pop()), "foofoo");
}

KAI_TESTS_END

#endif // KAI_UNIT_TESTS

//EOF
