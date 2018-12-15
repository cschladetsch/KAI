#include "TestCommon.h"
#include "MyTestStruct.h"
#include <KAI/Core/BuiltinTypes/List.h>

USING_NAMESPACE_KAI

using namespace std;


TEST(TestClassScripting, Test)
{
    // we can give the KAI runtime a custom allocator to use. we will just use
    // the standard one which uses malloc and free
    Console console;

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

    static bool trace = false;
    // can produce an XML representation
    if (trace) KAI_TRACE_1(mystruct.ToXmlString());

    // objects an be inserted into string streams
    StringStream stream;
    stream << mystruct;
    if (trace) KAI_TRACE_1(stream.ToString());

    // objects can also be inserted and extracted to/from binary streams
    // this is useful for networking and persistence.
    BinaryStream binary_stream(reg);
    binary_stream << mystruct;
    if (trace) KAI_TRACE_1(binary_stream);

    // accessing fields and methods uses pointer semantics
    mystruct->num = 345;
    mystruct->string = "hello world";
    mystruct->list = reg.New<List>();
    mystruct->list->Append(reg.New(42));
    mystruct->list->Append(reg.New(123));
    mystruct->list->Append(reg.New(String("foobar")));

    // see what it looks like
    if (trace) KAI_TRACE_1(mystruct.ToXmlString());

    // clear the stream, then see what mystruct looks like as a string
    stream.Clear();
    stream << mystruct;
    if (trace) KAI_TRACE_1(stream.ToString());

    // this part was also used in TestFunctionScripting, but the tests have been split up.
    // MyStruct will have to be put into a header.....

    //// objects are stored in a hierarchy; get the root of the hierarchy
    //// add a path to the root of hte tree, add mystruct as a child of the root
    //Object root = console.GetRoot();
    //console.GetTree().AddSearchPath(Pathname("/"));
    //root["mystruct"] = mystruct;
}
