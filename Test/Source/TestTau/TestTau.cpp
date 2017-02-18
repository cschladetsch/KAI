#include <KAI/Core/File.h>
#include <KAI/Language/Tau/TauParser.h>
#include <KAI/Language/Tau/Generate/Proxy.h>
#include <KAI/Language/Tau/Generate/Agent.h>

#include "TestCommon.h"

using namespace std;

USING_NAMESPACE_KAI

class TestTau : public TestCommon
{
protected:
	void AddrequiredClasses() override
	{
	}
};

TEST_F(TestTau, GenerateSimpleProxy0)
{
	auto inFileName = "Tau/t0.tau";
	auto outFileName = "Tau/t0.proxy.h";
	tau::Generate::Proxy proxy(inFileName, outFileName);
	ASSERT_FALSE(proxy.Failed) << proxy.Error;
}
