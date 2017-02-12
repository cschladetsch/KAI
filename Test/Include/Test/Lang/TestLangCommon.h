#include <KAI/KAI.h>
#include <KAI/Console/Console.h>
#include <gtest/gtest.h>
#include <memory>

KAI_BEGIN

class TestLangCommon : public ::testing::Test
{
protected:
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

	std::shared_ptr<Console> _console;
	Object _root;
	Registry *_reg;
	Stack *_data;
	const Stack *_context;
	Executor *_exec;
};

KAI_END
