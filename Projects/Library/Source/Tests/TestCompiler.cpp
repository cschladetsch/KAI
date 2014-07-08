
#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include "KAI/Tests/TestCompiler.h"
#	include "KAI/Console.h"

KAI_TESTS_BEGIN

void TestCompiler::Pre()
{
}

void TestCompiler::Post()
{
}

void TestCompiler::TestStatements()
{
	KAI_TEST_NOT_IMPLEMENTED();
}

void TestCompiler::TestExpressions()
{
	KAI_TEST_NOT_IMPLEMENTED();
}

void TestCompiler::TestConstruction()
{
	KAI_TEST_NOT_IMPLEMENTED();
}

void TestCompiler::TestComments()
{
	//Registry R;
	//Compiler C;
	//Pointer<Continuation> K;
	//R.AddClass<int>("int");
	//R.AddClass<Operation>("Operation");
	//R.AddClass<Array>("Array");
	//R.AddClass<Continuation>("Continuation");

	//K = C.Compile(R, "1");
	//KAI_TEST_TRUE(K->GetCode()->Size() == 1);

	//K = C.Compile(R, "// this is a comment");
	//KAI_TEST_TRUE(K->GetCode()->Size() == 0);

	//K = C.Compile(R, "//");
	//KAI_TEST_TRUE(K->GetCode()->Size() == 0);

	//K = C.Compile(R, 
	//	"42 // this is a comment"
	//	);
	//KAI_TEST_TRUE(K->GetCode()->Size() == 1);
	//KAI_TEST_TRUE(ConstDeref<int>(K->GetCode()->At(0)) == 42);

	//K = C.Compile(R, 
	//	"		// this is a comment \n\
	//	42\n\
	//	// this is another comment"
	//	);
	//KAI_TEST_TRUE(K->GetCode()->Size() == 1);
}

void TestCompiler::TestFunctions()
{
	Memory::StandardAllocator alloc;
	Console c(&alloc);
//
//	//auto e1 = c.ExecFile("Scripts/TestFun.kai");
//
//	auto f = c.Compile(
//		R"Q(
//
//fun f()
//	Print("Hello, World!")
//	a = 1+2*(3+4)
//	Assert(a == 15)
//	fun g(b)
//		return b+a
//	Assert(g(1) == 16)
//
//)Q"
//, Parser::ParseFunction);
//	KAI_TRACE_1(f);
//
//	//c.GetExecutor()->SetTraceLevel(100);
//	c.Execute(f);
//	c.Execute("f()");
//
	c.ExecuteFile("TestFun.kai");
	c.GetExecutor()->SetTraceLevel(100);
	c.Execute("fact(3)");
	KAI_TEST_EQUIV(ConstDeref<int>(c.GetExecutor()->GetDataStack()->Pop()), 3 * 2);

}

void TestCompiler::TestFloats()
{
	//Registry R;
	//Compiler C;
	//Pointer<Continuation> K;
	//R.AddClass<int>("int");
	//R.AddClass<float>("float");
	//R.AddClass<Operation>("Operation");
	//R.AddClass<Array>("Array");
	//R.AddClass<Continuation>("Continuation");

	//K = C.Compile(R, "1");
	//KAI_TEST_TRUE(K->GetCode()->Size() == 1);
	//KAI_TEST_EQUIV(K->GetCode()->At(0).GetTypeNumber(), Type::Traits<int>::Number);

	//K = C.Compile(R, "1.");
	//KAI_TEST_TRUE(K->GetCode()->Size() == 1);
	//KAI_TEST_EQUIV(K->GetCode()->At(0).GetTypeNumber(), Type::Traits<float>::Number);

	//K = C.Compile(R, "0.1");
	//KAI_TEST_TRUE(K->GetCode()->Size() == 1);
	//KAI_TEST_EQUIV(K->GetCode()->At(0).GetTypeNumber(), Type::Traits<float>::Number);
}

KAI_TESTS_END

#endif

//EOF
