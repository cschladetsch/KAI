#include "TestLangCommon.h"
#include "KAI/Language/Tau/TauParser.h"
#include "KAI/Language/Tau/Generate/GenerateProcess.h"
#include "KAI/Language/Tau/Generate/GenerateAgent.h"


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

    string out;
    tau::Generate::GenerateAgent gen(tauScript.c_str(), out);
    KAI_TRACE_1(out);
}

TEST_F(TestTau, TestAgentGen)
{
}
