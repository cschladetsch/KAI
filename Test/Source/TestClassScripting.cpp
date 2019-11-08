#include <KAI/Core/BuiltinTypes/List.h>

#include "TestCommon.h"
#include "MyTestStruct.h"

USING_NAMESPACE_KAI

using namespace std;

TEST(TestClassScripting, Test)
{
    // We can give the KAI runtime a custom allocator to use. we will just use
    // the standard one which uses malloc and free.
    Console console;

    // A registry is a factory for classes and instances.
    Registry &reg = console.GetRegistry();

    // Tell the registry about our structure.
    MyStruct::Register(reg);

    // Make a new instance. Note that we use a registry to make a new instance.
    // after that, we can use that instance to make other instances, which will just
    // use the registry that created it.
    Pointer<MyStruct> mystruct = reg.New<MyStruct>();

    ASSERT_TRUE(mystruct.Exists());
    EXPECT_EQ(mystruct.GetTypeNumber(), Type::Traits<MyStruct>::Number);

    static bool trace = true;
    // Can produce an XML representation.
	mystruct->num = 42;
	mystruct->string = "Freddy";
    if (trace)
		KAI_TRACE_1(mystruct.ToXmlString());
	/* Output:
	<Object type='MyStruct' name=''> <!-- no name since no in a parent dictionary -->
		<Property name='num'>42</Property>
		<Property name='string'>Freddy</Property>
	</Object>
	*/

    // Objects an be inserted into string streams.
    StringStream stream;
    stream << mystruct;
    if (trace)
		KAI_TRACE_1(stream.ToString());

    // Objects can also be inserted and extracted to/from binary streams
    // this is useful for networking and persistence.
    BinaryStream binary_stream(reg);
    binary_stream << mystruct;
    if (trace)
		KAI_TRACE_1(binary_stream);

    // Accessing fields and methods uses pointer semantics:
    mystruct->num = 345;
    mystruct->string = "hello world";
    mystruct->list = reg.New<List>();
    mystruct->list->Append(reg.New(42));
    mystruct->list->Append(reg.New(123));
    mystruct->list->Append(reg.New(String("foobar")));

    // See what it looks like.
    if (trace)
		KAI_TRACE_1(mystruct.ToXmlString());

    // Clear the stream, then see what mystruct looks like as a string:
    stream.Clear();
    stream << mystruct;
    if (trace)
		KAI_TRACE_1(stream.ToString());
}
