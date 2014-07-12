// (C) 2014 christian.schladetsch@gmail.com

#include <iostream>

#include "KAI/Console.h"

#ifdef KAI_UNIT_TESTS
#	include "KAI/Tests/TestAll.h"
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma comment(lib, "C:/Dev/KAI/Lib/Win32/Debug/library.lib")

KAI_BEGIN

void DebugTrace(const char *text)
{
	OutputDebugStringA(text);
	OutputDebugStringA("\n");
	//std::cerr << text << std::endl;
}

KAI_END

USING_NAMESPACE_KAI;

struct Colors
{
	enum What
	{
		Prompt,
		Input,
		Trace,
		Error,
		Warning,
		Last
	};
	WORD colors[Last];
	HANDLE hstdin, hstdout;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	Colors()
	{
		colors[Prompt] = 10;
		colors[Input] = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
		colors[Trace] = FOREGROUND_BLUE | FOREGROUND_GREEN;
		colors[Error] = 40;
		colors[Warning] = FOREGROUND_GREEN | FOREGROUND_RED;

		hstdin = GetStdHandle(STD_INPUT_HANDLE);
		hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

		// Remember how things were when we started
		GetConsoleScreenBufferInfo(hstdout, &csbi);
	}

	~Colors()
	{
		FlushConsoleInputBuffer(hstdin);
		SetConsoleTextAttribute(hstdout, csbi.wAttributes);
	}

	void SetColor(What c) const
	{
		SetConsoleTextAttribute(hstdout, colors[c]);
	}

	void operator()(What c) const { SetColor(c); }
};

void Run(Colors &c, Console &console)
{
	for (;;)
	{
		c(Colors::Prompt);
		std::cout << console.GetPrompt().c_str();
		c(Colors::Input);
		std::string text;
		std::getline(std::cin, text);
		c(Colors::Trace);
		std::cout << console.Process(text.c_str()).c_str();
	}
}

void RunTests(Console &console);

int main(int argc, char **argv)
{
	Colors c;
	nstd::vector<String> args;
	for (int N = 1; N < argc; ++N)
		args.push_back(argv[N]);
	
	Memory::StandardAllocator alloc;
	Console console(args, &alloc);

	c(Colors::Warning);

#if defined(PROFILE)
	for (int n = 0; n < 10; ++n)
#else
		RunTests(console);
#endif

#if !defined(PROFILE)
	Run(c, console);
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
