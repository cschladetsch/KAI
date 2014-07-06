// (C) christian.schladetsch@gmail.com

#include "Precompiled.h"
#include "gtest/gtest.h"
#include <fstream>

#include "Lexer.h"
#include "Parser.h"
#include "Translator.h"

#pragma comment(lib, "C:/Dev/KAI/Lib/Win32/Debug/library.lib")

KAI_BEGIN

void DebugTrace(const char *text)
{
	std::cout << text;
}

KAI_END

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

