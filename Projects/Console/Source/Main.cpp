#include <iostream>
#include "KAI/ExecutorPCH.h"
#include "KAI/ConsoleColor.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

KAI_BEGIN

void DebugTrace(const char *text)
{
#ifdef WIN32
	OutputDebugStringA(text);
	OutputDebugStringA("\n");
#endif

	std::cerr << text << std::endl;
}

KAI_END

USING_NAMESPACE_KAI

static Color _color;

int main(int argc, char **argv)
{
	KAI_UNUSED_2(argc, argv);

	Console console(std::make_shared<Memory::StandardAllocator>());
	console.SetLanguage(Language::Pi);


	Process::trace = 0;
	console.GetExecutor()->SetTraceLevel(0);

	return console.Run();
}

