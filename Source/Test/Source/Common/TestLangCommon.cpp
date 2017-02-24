#include "TestLangCommon.h"

KAI_BEGIN

using namespace std;

void TestLangCommon::SetUp()
{
	_reg = &_console.GetRegistry();
	_exec = &_console.GetExecutor();
	_data = &*_exec->GetDataStack();
	_context = &*_exec->GetContextStack();
	_tree = &_console.GetTree();
	_root = _tree->GetRoot();
}

void TestLangCommon::TearDown()
{
}

KAI_END
