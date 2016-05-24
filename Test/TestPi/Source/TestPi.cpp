#include "Main.h"

using namespace kai;
using namespace std;

class TestPi : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		_console = make_unique<Console>();
		_console->SetLanguage(Language::Pi);
		_reg = &_console->GetRegistry();
		_root = _console->GetRoot();
		_exec = &*_console->GetExecutor();
		_data = &*_exec->GetDataStack();
		_context = &*_exec->GetContextStack();

		_exec->SetTraceLevel(10);
		TranslatorCommon::trace = 2;
	}

	virtual void TearDown()
	{
	}

	template<class T>
	const T &AtData(int index)
	{
		return Deref<T>(_data->At(index));
	}

	Continuation const&GetContext()
	{
		return ConstDeref<Continuation>(_context->At(0));
	}

	template <class T>
	void AsssertResult(const char *text, T const &val)
	{
		_data->Clear();
		_console->Execute(text);
		ASSERT_EQ(AtData<T>(0), val);
	}

	unique_ptr<Console> _console;
	Object _root;
	Registry *_reg;
	Stack *_data;
	const Stack *_context;
	Executor *_exec;
};

TEST_F(TestPi, TestArithmetic)
{
	TranslatorCommon::trace = 1;

	_data->Clear();
	_console->Execute("1 2 +");
	ASSERT_EQ(AtData<int>(0), 3);

	_data->Clear();
	_console->Execute("1 2 -");
	ASSERT_EQ(AtData<int>(0), -1);

	_data->Clear();
	_console->Execute("2 2 *");
	ASSERT_EQ(AtData<int>(0), 4);

	_data->Clear();
	_console->Execute("6 2 div");
	ASSERT_EQ(AtData<int>(0), 3);

	_data->Clear();
	_console->Execute("3 2 + 2 2 * * 2 div");
	ASSERT_EQ(AtData<int>(0), 10);
}

TEST_F(TestPi, TestAssignment)
{
	_console->SetLanguage(Language::Pi);
	_console->Execute("'a 3 :=");

	Label name("a");
	Object scope = GetContext().GetScope();
	ASSERT_TRUE(scope.HasChild(name));
	ASSERT_EQ(Deref<int>(scope.GetChild(name)), 3);
}


