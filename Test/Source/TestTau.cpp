#include "./Common.h"
#include <KAI/Core/File.h>

using namespace std;

USING_NAMESPACE_KAI

class TestTau : public KAITestClass
{
protected:
	void AddrequiredClasses() override
	{
	}
};

TEST_F(TestTau, GenerateSimpleProxy0)
{
	auto contents = ReadTextFile("Tau/t0.tau");
	cout << contents << endl;
}
