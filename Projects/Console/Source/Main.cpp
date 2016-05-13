#include <iostream>
#include "KAI/KAI.h"
#include "KAI/GetStorageBase.h"
#include "KAI/StringStream.h"
#include "KAI/Object.h"
#include "KAI/GetStorageBase.h"
#include "KAI/Registry.h"
#include "KAI/ExecutorPCH.h"

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

	Registry reg(std::make_shared<Memory::StandardAllocator>());
	reg.AddClass<Console>("Console");
	Console &console = Deref<Console>(reg.New<Console>());
	console.SetLanguage(Language::Pi);

	Process::trace = 0;
	console.GetExecutor()->SetTraceLevel(0);

	return console.Run();
}

