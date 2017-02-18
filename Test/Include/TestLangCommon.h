#include <memory>
#include <gtest/gtest.h>

#include <KAI/KAI.h>
#include <KAI/Console/Console.h>

#include "./TestCommon.h"

KAI_BEGIN

// Common for tests that work with the various
// languages in the system.
//
// TestLangCommon provides a fully-functional
// working test envionment with an interactive
// console and direct access to the data and 
// context stacks for the Executor
class TestLangCommon : public TestCommon
{
protected:
	void SetUp();
	void TearDown();

	// Get const ref to data at index on stack
	template<class T>
	const T &AtData(int index)
	{
		return Deref<T>(_data->At(index));
	}

	// get the curent continuation context
	Continuation const& GetContext()
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

protected:
	std::shared_ptr<Console> _console;
	Object _root;
	Registry *_reg;
	Stack *_data;
	const Stack *_context;
	Executor *_exec;
};

KAI_END
