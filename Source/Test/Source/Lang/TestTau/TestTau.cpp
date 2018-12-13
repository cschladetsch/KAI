#include "TestLangCommon.h"
#include "KAI/Language/Tau/TauParser.h"
#include "KAI/Language/Tau/Generate/GenerateProcess.h"
#include "KAI/Language/Tau/Generate/GenerateAgent.h"
#include "KAI/Language/Tau/Generate/GenerateProxy.h"


using namespace kai;
using namespace std;

struct TestTau : TestLangCommon
{
};

TEST_F(TestTau, TestProxyGen)
{
    auto tauScript = LoadScriptText("Tau2.tau");
    ASSERT_FALSE(tauScript.empty());

    Console console;
    Registry &reg = console.GetRegistry();

    string agentText, proxyText;
    tau::Generate::GenerateProxy proxy(tauScript.c_str(), proxyText);
    tau::Generate::GenerateAgent agent(tauScript.c_str(), agentText);

    KAI_TRACE_1(proxyText);
    KAI_TRACE_1(agentText);
}

TEST_F(TestTau, TestAgentGen)
{
}
