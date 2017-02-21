#include "TestLangCommon.h"
#include <KAI/Core/BuiltinTypes.h>
#include <KAI/Core/BuiltinTypes/Array.h>

using namespace kai;
using namespace std;

struct TestPi : TestLangCommon
{
};

TEST_F(TestPi, TestComments)
{
	_console->SetLanguage(Language::Pi);
	_console->Execute("// text\n\n\n\n\n");
	ASSERT_EQ(_data->Size(), 0);

	_console->Execute("// text\n");
	ASSERT_EQ(_data->Size(), 0);
	
	_console->Execute("// text");
	ASSERT_EQ(_data->Size(), 0);

	_console->Execute("//");
	ASSERT_EQ(_data->Size(), 0);
}

TEST_F(TestPi, TestArithmetic)
{
	// TranslatorCommon::trace = 1;

	_console->SetLanguage(Language::Pi);
	_data->Clear();
	_console->Execute("6 2 div");
	ASSERT_EQ(AtData<int>(0), 3);

	_console->Execute("1 2 +");
	auto result = AtData<int>(0);
	ASSERT_EQ(result, 3);

	_data->Clear();
	_console->Execute("1 2 -");
	ASSERT_EQ(AtData<int>(0), -1);

	_data->Clear();
	_console->Execute("2 2 *");
	ASSERT_EQ(AtData<int>(0), 4);

	_data->Clear();
	_console->Execute("3 2 + 2 2 * * 2 div");
	ASSERT_EQ(AtData<int>(0), 10);
}

TEST_F(TestPi, TestVectors)
{
	_console->Execute("[] type");
	Object ty = _data->At(0);
	ASSERT_EQ(ty.GetTypeNumber(), Type::Traits<Array>::Number);

	_data->Clear();
	_console->Execute("[1 2 3] size &");
	ASSERT_EQ(AtData<int>(0), 3);

	_data->Clear();
	_console->Execute("1 2 3 3 >array");
	Pointer<Array> a = _data->Top();
	ASSERT_EQ(a->Size(), 3);
	ASSERT_EQ(ConstDeref<int>(a->At(0)), 1);
	ASSERT_EQ(ConstDeref<int>(a->At(1)), 2);
	ASSERT_EQ(ConstDeref<int>(a->At(2)), 3);

}

