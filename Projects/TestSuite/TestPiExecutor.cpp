
#include "TestBase.h"
#include "KAI/Console.h"
#include "KAI/Translator/Pi/Pi.h"

using namespace kai;
using namespace std;

TEST(TestPiExec, ExecArith)
{
	auto input = "1 1 +";

	Registry r;
	r.AddClass<void>();
	r.AddClass<int>();
	r.AddClass<String>();
	r.AddClass<Executor>();

	auto lex = make_shared<PiLexer>(input);
	lex->Process();
	auto parse = make_shared<PiParser>(lex);
	auto trans = make_shared<PiTranslator>(parse, r);

	auto e = r.New<Executor>();
}
