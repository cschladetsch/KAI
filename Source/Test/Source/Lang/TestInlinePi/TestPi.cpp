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

TEST_F(TestPi, TestVectors)
{
	_console->SetLanguage(Language::Pi);

	_data->Clear();
	_console->Execute("1 1 toarray");
	Pointer<Array> a = _data->Top();
	ASSERT_EQ(a->Size(), 1);
	ASSERT_EQ(ConstDeref<int>(a->At(0)), 1);

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
}
*/

TEST_F(TestPi, TestScope)
{
	try {
	// store to explicit root of tree
	_console->Execute("42 '/b #");
	const Label b("b");
	ASSERT_TRUE(_root->Has(b));
	ASSERT_EQ(42, ConstDeref<int>(_root.Get(b)));
	
	// store into local scope. whatever that is supposed to be?
	// currently it is the scope of the continuation that is passed to the 
	// executor --- so it goes out of scope immediately after the continuation 
	// has finished (and before the next starts)
	//
	// we don't want that (or do we). Rather, the *first* continuation should run in the
	// current scope of the executor - after that, it uses local scope
	_console->Execute("1 'a #");
	// ASSERT_TRUE(_tree->GetScope()->Has(Label("a")));
	}
	catch (Exception::Base &e)
	{
		ASSERT_TRUE(false) << e.ToString().c_str();
	}
	
}

