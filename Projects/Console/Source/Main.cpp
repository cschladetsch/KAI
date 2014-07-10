#include <iostream>

#include "KAI/Console.h"

#ifdef KAI_UNIT_TESTS
#include "KAI/Tests/TestAll.h"
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma comment(lib, "C:/Dev/KAI/Lib/Win32/Debug/library.lib")

KAI_BEGIN

void DebugTrace(const char *text)
{
	OutputDebugStringA(text);
	OutputDebugStringA("\n");
	std::cerr << text << std::endl;
}

KAI_END

USING_NAMESPACE_KAI;

void Run(Console &console)
{
	for (;;)
	{
		std::cout << console.GetPrompt().c_str();
		std::string text;
		std::getline(std::cin, text);
		std::cout << console.Process(text.c_str()).c_str();
	}
}

void RunTests(Console &console);

int main(int argc, char **argv)
{
	nstd::vector<String> args;
	for (int N = 1; N < argc; ++N)
		args.push_back(argv[N]);
	
	Memory::StandardAllocator alloc;
	Console console(args, &alloc);

#ifdef PROFILE
	for (int n = 0; n < 10; ++n)
		RunTests(console);
#else
	RunTests(console);
	Run(console);
#endif
}

void RunTests(Console &console)
{
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
	//ADD_TEST(TestObject);

	Pointer<Test::BasicOutput> out = console.GetRegistry().New<Test::BasicOutput>();
	module.Run(out);
	std::cout << out->GetSummary().ToString().c_str() << std::endl;
}

//EOF
