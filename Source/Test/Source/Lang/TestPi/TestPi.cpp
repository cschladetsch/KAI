#include "TestLangCommon.h"
#include <KAI/Core/BuiltinTypes.h>
#include <KAI/Core/BuiltinTypes/Array.h>

using namespace kai;
using namespace std;

struct TestPi : TestLangCommon
{
};

TEST_F(TestPi, TestContinuations)
{
    auto exec = _console.GetExecutor();
    auto data = exec->GetDataStack();
    auto context = exec->GetContextStack();

    _console.GetExecutor()->SetTraceLevel(999);
    _console.SetLanguage(Language::Pi);
    _console.Execute("1");
    _console.Execute("1 2");
    _console.Execute("{ } &");
    _console.Execute("{ { } & } &");

    ASSERT_EQ(data->Size(), 0);
    ASSERT_EQ(context->Size(), 0);
}

TEST_F(TestPi, TestComments)
{
    _console.SetLanguage(Language::Pi);
    _console.Execute("// text\n\n\n\n\n");
    ASSERT_EQ(_data->Size(), 0);

    _console.Execute("// text\n");
    ASSERT_EQ(_data->Size(), 0);

    _console.Execute("// text");
    ASSERT_EQ(_data->Size(), 0);

    _console.Execute("//");
    ASSERT_EQ(_data->Size(), 0);
}

// Create some stuff on the stack, freeze it to a binary packet, thaw it out to objects,
// then ensure the end result is what went in.
TEST_F(TestPi, TestFreezeThaw)
{
    _console.SetLanguage(Language::Pi);
    _console.Execute("42 \"hello\" [3 9 8] 3 toarray freeze");
    auto const &packet = ConstDeref<BinaryStream>(_data->Top());
    TEST_COUT << "Frozen to " << packet.Size() << " bytes";
    _console.Execute("thaw");

    // the 'thaw' command will put all contents onto the stack
    auto stack = ConstDeref<Array>(_data->Top());
    ASSERT_EQ(3, stack.Size());
    auto a = stack.At(2);
    auto b = stack.At(1);
    auto c = stack.At(0);

    ASSERT_TRUE(a.IsType<Array>());
    ASSERT_TRUE(b.IsType<String>());
    ASSERT_TRUE(c.IsType<int>());

    auto a1 = ConstDeref<Array>(a);
    auto b1 = ConstDeref<String>(b);
    auto c1 = ConstDeref<int>(c);

    ASSERT_EQ(a1.Size(), 3);
    ASSERT_EQ(ConstDeref<int>(a1.At(0)), 3);
    ASSERT_EQ(ConstDeref<int>(a1.At(1)), 9);
    ASSERT_EQ(ConstDeref<int>(a1.At(2)), 8);
    ASSERT_EQ(b1, "hello");
    ASSERT_EQ(c1, 42);
}

TEST_F(TestPi, TestArithmetic)
{
    _console.SetLanguage(Language::Pi);
    _data->Clear();
    _console.Execute("6 2 div");
    ASSERT_EQ(AtData<int>(0), 3);

    _console.Execute("1 2 +");
    auto result = AtData<int>(0);
    ASSERT_EQ(result, 3);

    _data->Clear();
    _console.Execute("1 2 -");
    ASSERT_EQ(AtData<int>(0), -1);

    _data->Clear();
    _console.Execute("2 2 *");
    ASSERT_EQ(AtData<int>(0), 4);

    _data->Clear();
    _console.Execute("3 2 + 2 2 * * 2 div");
    ASSERT_EQ(AtData<int>(0), 10);
}

TEST_F(TestPi, TestVectors)
{
    _console.SetLanguage(Language::Pi);

    _data->Clear();
    _console.Execute("1 1 toarray");
    Pointer<Array> a = _data->Top();
    ASSERT_EQ(a->Size(), 1);
    ASSERT_EQ(ConstDeref<int>(a->At(0)), 1);

    _console.Execute("[]");
    Pointer<Array> array = _data->At(0);
    ASSERT_TRUE(array.Exists());
    ASSERT_TRUE(array->Empty());
    ASSERT_EQ(array->Size(), 0);

    _data->Clear();
    _console.Execute("[1 2 3]");
    array = _data->At(0);
    ASSERT_TRUE(array.Exists());
    ASSERT_FALSE(array->Empty());
    ASSERT_EQ(array->Size(), 3);
    ASSERT_EQ(ConstDeref<int>(array->At(0)), 1);
    ASSERT_EQ(ConstDeref<int>(array->At(1)), 2);
    ASSERT_EQ(ConstDeref<int>(array->At(2)), 3);

    _data->Clear();
    _console.Execute("[1 2 3] size");
    ASSERT_EQ(AtData<int>(0), 3);
}

TEST_F(TestPi, TestScope)
{
    const Label b("b");
    const Label c("c");

    Pathname p("'/b");
    EXPECT_TRUE(p.Validate());

    // store to explicit root of tree
    _console.Execute("42 '/b #");
    ASSERT_TRUE(_root.Has(b));
    ASSERT_EQ(42, ConstDeref<int>(_root.Get(b)));

    // this is only storing to local scope - within the context
    // of the command-line in which it was executed - so it
    // should not be preserved in the global tree
    _console.Execute("1 'c #");
    ASSERT_FALSE(_root.Has(c));
}
