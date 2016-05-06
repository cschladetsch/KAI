#include "KAI/ExecutorPCH.h"
#include "KAI/ConsoleColor.h"

KAI_BEGIN

#ifdef WIN32
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>

static WORD colors[Color::Last];
static HANDLE hstdout;
static CONSOLE_SCREEN_BUFFER_INFO orig;
#endif


Color::Color()
{
	colors[Prompt] = FOREGROUND_GREEN;
	colors[Input] = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	colors[Trace] = FOREGROUND_BLUE | FOREGROUND_GREEN;
	colors[Error] = FOREGROUND_RED;
	colors[Warning] = FOREGROUND_GREEN | FOREGROUND_RED;

	hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

	// Remember how things were when we started
	GetConsoleScreenBufferInfo(hstdout, &orig);
}

Color::~Color()
{
	SetConsoleTextAttribute(hstdout, orig.wAttributes);
}

void Color::SetColor(Type c)
{
	SetConsoleTextAttribute(hstdout, colors[c]);
}

KAI_END

