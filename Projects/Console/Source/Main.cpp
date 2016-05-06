#include <iostream>
#include "KAI/ExecutorPCH.h"

KAI_BEGIN

void DebugTrace(const char *text)
{
	//OutputDebugStringA(text);
	//OutputDebugStringA("\n");
	std::cerr << text << std::endl;
}

KAI_END

USING_NAMESPACE_KAI

int main(int argc, char **argv)
{
	Memory::StandardAllocator alloc;
	Registry r(&alloc);

	std::vector<String> args;
	for (int N = 1; N < argc; ++N)
		args.push_back(argv[N]);
	
	Console console(args, &alloc);
	console.SetLanguage(Language::Pi);

	Process::trace = 1;
	console.GetExecutor()->SetTraceLevel(1);

	console.Run();
}

