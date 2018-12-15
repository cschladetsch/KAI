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

    string output;
    tau::Generate::GenerateAgent agent(tauScript.c_str(), output);

    if (agent.Failed)
        KAI_TRACE_ERROR_1(agent.Error);

    KAI_TRACE_1(output);
}

TEST_F(TestTau, TestAgentGen)
{
    auto tauScript = LoadScriptText("Tau2.tau");
    ASSERT_FALSE(tauScript.empty());

    string output;
    tau::Generate::GenerateProxy proxy(tauScript.c_str(), output);

    KAI_TRACE_1(output);
}
