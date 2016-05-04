
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

	auto trans = make_shared<PiTranslator>(r);
	auto cont = trans->Translate(input, Structure::Sequence);
	Pointer<Executor> e = r.New<Executor>();
	e->Continue(cont);
	auto stack = e->GetDataStack();
	//ASSERT_EQ(e->GetDataStack())

}

TEST(TestPiExec, TestConsole)
{
	Memory::BaseAllocator alloc;
	Console console(&alloc);

	//console.SetPi();
	//console.SetRho();

}
