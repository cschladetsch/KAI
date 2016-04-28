#include <iostream>
#include "KAI/Console.h"

#ifdef KAI_UNIT_TESTS
#	include "KAI/Tests/TestAll.h"
#endif

//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>

KAI_BEGIN

void DebugTrace(const char *text)
{
	//OutputDebugStringA(text);
	//OutputDebugStringA("\n");
	std::cerr << text << std::endl;
}

KAI_END

USING_NAMESPACE_KAI

//error LNK2001 : unresolved external symbol "public: static char const * const kai::Type::TraitsBase<class kai::Array,28,39619,class kai::Array,class kai::Array &,class kai::Array const &>::_name" (? _name@?$TraitsBase@VArray@kai@@$0BM@$0JKMD@V12@AAV12@ABV12@@Type@kai@@

void RunTests(Console &console);

int main(int argc, char **argv)
{
	Memory::StandardAllocator alloc;
	Registry r(&alloc);

	std::vector<String> args;
	for (int N = 1; N < argc; ++N)
		args.push_back(argv[N]);
	
	Console console(args, &alloc);

#if defined(PROFILE)
	for (int n = 0; n < 10; ++n)
#endif
		RunTests(console);

#if !defined(PROFILE)
	console.Run();
#endif
}

void RunTests(Console &console)
{
#if defined(KAI_UNIT_TESTS)
	#define ADD_TEST(Name) \
		module.AddSuite<Tests::Name>(#Name);

	Test::Module module("AutoRun");
	ADD_TEST(TestTest);
	ADD_TEST(TestRegistry);
	ADD_TEST(TestTree);
	ADD_TEST(TestTriColor);
	ADD_TEST(TestCompiler);
	ADD_TEST(TestEvents);
	ADD_TEST(TestFunction);
	ADD_TEST(TestRegistry);
	//ADD_TEST(TestObject);

	Pointer<Test::BasicOutput> out = console.GetRegistry().New<Test::BasicOutput>();
	module.Run(out);
	std::cout << out->GetSummary().ToString().c_str() << std::endl;
#endif
}

//EOF
