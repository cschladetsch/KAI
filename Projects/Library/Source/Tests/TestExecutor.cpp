
#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include "KAI/Tests/TestExecutor.h"

KAI_TESTS_BEGIN

void TestExecutor::Pre()
{
	registry.AddClass<int>("int");
	registry.AddClass<String>("String");
	registry.AddClass<Stack>("Stack");
	registry.AddClass<Array>("Array");
	registry.AddClass<Continuation>("Continuation");
	registry.AddClass<Compiler>("Compiler");
	registry.AddClass<Operation>("Operation");
	registry.AddClass<Executor>("Executor");
	registry.AddClass<Pathname>("Pathname");

	executor = registry.New<Executor>();
	compiler = registry.New<Compiler>();
}

void TestExecutor::Post()
{
	//registry.Clear();
}

void TestExecutor::TestConstruction()
{
}

void TestExecutor::TestStringLiterals()
{
	continuation = compiler->Compile(registry, "'/foo/bar");
	KAI_TEST_TRUE(continuation->HasCode());
	executor->Continue(continuation);

	continuation = compiler->Compile(registry, "\"string literal\"");
	executor->Continue(continuation);

	KAI_TEST_EQUIV(ConstDeref<String>(executor->Pop()), "string literal");
	KAI_TEST_EQUIV(ConstDeref<Pathname>(executor->Pop()).ToString(), "'/foo/bar");
}

void TestExecutor::TestContinuations()
{
	continuation = compiler->Compile(registry, "1 2 +");
	executor->Continue(continuation);

	Pointer<int> N = executor->Pop();
	KAI_TEST_EQUIV(*N, 3);
}

KAI_TESTS_END

#endif

//EOF
