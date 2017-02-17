#include "./Common.h"
#include <KAI/Core/File.h>
#include <KAI/Language/Tau/TauParser.h>
#include <KAI/Language/Tau/Generate/Proxy.h>
#include <KAI/Language/Tau/Generate/Agent.h>

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
	auto inFileName = "Tau/t0.tau";
	auto outFileName = "Tau/t0.out.h";
	tau::Generate::Proxy(inFileName, outFileName);
}
