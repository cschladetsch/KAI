#include "TestBase.h"
#include "KAI/Console.h"

using namespace kai;
using namespace std;

// this is a C++ struct that we will reflect to access at runtime via scripting
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

	mutable bool called[3];

	int Method0()
	{
		called[0] = true;
		return 42;
	}

	String Method1(int n, String str) const
	{
		called[1] = true;
		KAI_TRACE_1(str);
		String result;
		while (n-- > 0)
			result += str;
		return result;
	}

	void Method2()
	{
		called[2] = true;
	}

	// every class that is to be reflected must be registered.
	// by convention, this is done in a static function called Register
	static void Register(Registry &);
};

// register the type
KAI_BEGIN
KAI_TYPE_TRAITS(MyStruct, 555, Properties::Nothing);
KAI_END

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
			//("list", &MyStruct::list, "a list")
		;
}

TEST(TestClassScripting, Test)
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

	ASSERT_TRUE(mystruct.Exists());
	EXPECT_EQ(mystruct.GetTypeNumber(), Type::Traits<MyStruct>::Number);

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
	mystruct->list->Append(reg.New(String("foobar")));

	// see what it looks like
	KAI_TRACE_1(mystruct.ToXmlString());

	// clear the stream, then see what mystruct looks like as a string
	stream.Clear();
	stream << mystruct;
	KAI_TRACE_1(stream.ToString());

	// this part was also used in TestFunctionScripting, but the tests have been split up.
	// MyStruct will have to be put into a header.....

	//// objects are stored in a hierarchy; get the root of the hierarchy
	//// add a path to the root of hte tree, add mystruct as a child of the root
	//Object root = console.GetRoot();
	//console.GetTree().AddSearchPath(Pathname("/"));
	//root["mystruct"] = mystruct;
}