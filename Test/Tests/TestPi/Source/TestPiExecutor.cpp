#include "Test/Common.h"

using namespace kai;
using namespace std;

TEST(TestPiExec, ExecArith)
{
	auto input = "1 1 +";

	Registry r;
	r.AddClass<void>();
	r.AddClass<bool>();
	r.AddClass<int>();
	r.AddClass<String>();
	r.AddClass<Array>();
	r.AddClass<Stack>();
	r.AddClass<Executor>();
	r.AddClass<Continuation>();
	r.AddClass<Operation>();

	auto trans = make_shared<PiTranslator>(r);
	auto cont = trans->Translate(input, Structure::Sequence);
	auto code = cont->GetCode();
	KAI_TRACE_1(code);

	Pointer<Executor> e = r.New<Executor>();
	e->Continue(cont);
	auto stack = e->GetDataStack();
	//ASSERT_EQ(e->GetDataStack())
}
