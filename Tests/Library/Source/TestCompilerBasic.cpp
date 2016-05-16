
#include "TestBase.h"

#ifdef TEST_COMPILER_BASIC

USING_NAMESPACE_KAI
using namespace std;

class TestExecutor : public ::testing::Test
{
protected:
	Registry reg;
	Pointer<Compiler> compiler;
	Pointer<Executor> executor;
	Pointer<Continuation> continuation;

protected:
	virtual void SetUp()
	{
		//Memory::BaseAllocator alloc);
		//reg = new Registry(&alloc);

		reg.AddClass<int>("int");
		reg.AddClass<String>("String");
		reg.AddClass<Stack>("Stack");
		reg.AddClass<Array>("Array");
		reg.AddClass<Continuation>("Continuation");
		reg.AddClass<Compiler>("Compiler");
		reg.AddClass<Operation>("Operation");
		reg.AddClass<Executor>("Executor");
		reg.AddClass<Pathname>("Pathname");

		executor = reg.New<Executor>();
		compiler = reg.New<Compiler>();
	}

	Value<Stack> Exec(const char *script)
	{
		continuation = compiler->Compile(reg, script);
		executor->Continue(continuation);
		return executor->GetDataStack();
	}

	template <class T>
	bool Eval(const char *script, const T& val)
	{
		continuation = compiler->Compile(reg, script);
		executor->Continue(continuation);
		Value<Stack> stack = executor->GetDataStack();
		if (stack->Size() != 1)
			return false;
		return ConstDeref<T>(stack->Pop()) == val;
	}

public:
	void TestArithmetic();
	void TestStringLiterals();
};

TEST_F(TestExecutor, TestArithmetic)
{
	EXPECT_TRUE(Eval<int>("1", 1));
	EXPECT_TRUE(Eval<int>("2", 2));
	EXPECT_TRUE(Eval<int>("1 2 +", 3));
	EXPECT_TRUE(Eval<int>("2 3 mul", 6));
	EXPECT_TRUE(Eval<int>("8 4 div", 2));
	EXPECT_TRUE(Eval<int>("1 2 3 4 + + +", 10));
	EXPECT_TRUE(Eval<int>("1 2 + 3 *", 6));
}

TEST_F(TestExecutor, TestStringLiterals)
{
	continuation = compiler->Compile(reg, "'foo.'bar");
	ASSERT_TRUE(continuation->HasCode());
	executor->Continue(continuation);

	continuation = compiler->Compile(reg, "\"string literal\"");
	executor->Continue(continuation);

	EXPECT_EQ(ConstDeref<String>(executor->Pop()), "string literal");
	EXPECT_EQ(ConstDeref<Pathname>(executor->Pop()).ToString(), "'/foo/bar");
}

#endif

