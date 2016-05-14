#include "KAI/KAI.h"
#include "KAI/ExecutorPCH.h"

#include <iostream>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

using namespace std;

KAI_BEGIN

void DebugTrace(const char *text)
{
#ifdef WIN32
	OutputDebugStringA(text);
	OutputDebugStringA("\n");
#endif

	cerr << text << endl;
}

KAI_END

USING_NAMESPACE_KAI


static Color _color;

int main(int argc, char **argv)
{
	KAI_UNUSED_2(argc, argv);

	cout << "pyro v0.1" << endl;

	Registry reg(make_shared<Memory::StandardAllocator>());
	reg.Clear();

	cout << "&reg=" << &reg << endl;

	cout << "add int\n";
	auto label = Label("Signed32");
	reg.AddClass<int>(label);

	cout << "add Console\n";
	reg.AddClass<Console>(Label("Console"));
	Console &console = Deref<Console>(reg.New<Console>());
	console.SetLanguage(Language::Pi);

	Process::trace = 0;
	console.GetExecutor()->SetTraceLevel(0);

	return console.Run();
}

