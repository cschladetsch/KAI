#include "TestBase.h"

using namespace kai;
using namespace std;

namespace
{
	TEST(PiParser, TestSimple)
	{
		auto input = "1";

		auto lex = make_shared<PiLexer>(input);
		auto parse = make_shared<PiParser>(lex, Structure::Sequence);

		ASSERT_FALSE(lex->Failed);
		ASSERT_FALSE(parse->Failed);

		KAI_TRACE_1(parse->Print());
	}
}
