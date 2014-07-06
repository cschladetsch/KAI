// (C) christian.schladetsch@gmail.com

#include "Precompiled.h"
#include "gtest/gtest.h"
#include <fstream>

#include "Lexer.h"
#include "Parser.h"
#include "Translator.h"

using namespace std;
using namespace KAI_TRANS_NAMESPACE_NAME;

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

