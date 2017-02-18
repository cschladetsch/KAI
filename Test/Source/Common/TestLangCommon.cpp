#include <KAI/Console.h>

#include "TestLangCommon.h"

USING_NAMESPACE_KAI

using namespace std;

void TestLangCommon::SetUp()
{
	TestCommon::SetUp();

	_reg = &reg;
	_root = tree.GetRoot();
	_console = make_shared<Console>();
	_exec = &*_console->GetExecutor();
	_data = &*_exec->GetDataStack();
	_context = &*_exec->GetContextStack();
}

void TestLangCommon::TearDown()
{
	TestCommon::TearDown();
}
