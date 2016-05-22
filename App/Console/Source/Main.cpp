#include "KAI/Console/Console.h"

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
#else
	cerr << text << endl;
#endif
}

KAI_END

USING_NAMESPACE_KAI

static Color _color;

int main(int argc, char **argv)
{
	KAI_UNUSED_2(argc, argv);

	cout << "pyro v0.1" << endl;

	Console console;
	console.SetLanguage(Language::Pi);

	Process::trace = 0;
	console.GetExecutor()->SetTraceLevel(0);

	return console.Run();
}

