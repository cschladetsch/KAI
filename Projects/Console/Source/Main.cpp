
#include "KAI/KAI.h"
#include "KAI/Tree.h"
#include "KAI/BuiltinTypes/Array.h"
#include "KAI/Continuation.h"
#include "KAI/Value.h"
#include "KAI/Operation.h"
#include "KAI/Executor.h"
#include "KAI/Compiler.h"
#include "KAI/Memory.h"
#include "KAI/Console.h"

#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma comment(lib, "C:/Dev/KAI/Lib/Win32/Debug/library.lib")

KAI_BEGIN

void DebugTrace(const char *text)
{
	OutputDebugStringA(text);
	OutputDebugStringA("\n");
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

int main(int argc, char **argv)
{
	nstd::vector<String> args;
	for (int N = 1; N < argc; ++N)
		args.push_back(argv[N]);
	
	Memory::StandardAllocator alloc;
	Console console(args, &alloc);
	std::cout << console.WriteStack().c_str();
	Run(console);
}

//EOF
