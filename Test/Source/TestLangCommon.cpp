#include "Test/TestLangCommon.h"

using namespace std;

KAI_BEGIN

void TestLangCommon::SetUp()
{
	_console = make_shared<Console>();
	_console->SetLanguage(Language::Pi);
	_reg = &_console->GetRegistry();
	_root = _console->GetRoot();
	_exec = &*_console->GetExecutor();
	_data = &*_exec->GetDataStack();
	_context = &*_exec->GetContextStack();

	_exec->SetTraceLevel(10);
	TranslatorCommon::trace = 2;
}

void TearDown()
{
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

KAI_END

