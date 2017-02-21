#include "TestLangCommon.h"
#include <KAI/Core/BuiltinTypes.h>
#include <KAI/Core/BuiltinTypes/Array.h>

#include <iostream>

using namespace kai;
using namespace std;

struct TestPi : TestLangCommon
{
};

/*
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
*/

TEST_F(TestPi, TestVectors)
{
	// try
	// {
		_console->SetLanguage(Language::Pi);
		_console->Execute("[]");
		Pointer<Array> array = _data->At(0);
		ASSERT_TRUE(array.Exists());
		ASSERT_TRUE(array->Empty());
		ASSERT_EQ(array->Size(), 0);

		_data->Clear();
		_console->Execute("[1 2 3]");
		array = _data->At(0);
		ASSERT_TRUE(array.Exists());
		ASSERT_FALSE(array->Empty());
		ASSERT_EQ(array->Size(), 3);
		ASSERT_EQ(ConstDeref<int>(array->At(0)), 1);
		ASSERT_EQ(ConstDeref<int>(array->At(1)), 2);
		ASSERT_EQ(ConstDeref<int>(array->At(2)), 3);

		_data->Clear();
		_console->Execute("[1 2 3] size");
		ASSERT_EQ(AtData<int>(0), 3);

		_data->Clear();
		_console->Execute("1 2 3 3 >array");
		Pointer<Array> a = _data->Top();
		ASSERT_EQ(a->Size(), 3);
		ASSERT_EQ(ConstDeref<int>(a->At(0)), 1);
		ASSERT_EQ(ConstDeref<int>(a->At(1)), 2);
		ASSERT_EQ(ConstDeref<int>(a->At(2)), 3);
	// }
	// catch (Exception::Base &e)
	// {
	// 	cerr << e.ToString().c_str() << endl;
	// }
}

