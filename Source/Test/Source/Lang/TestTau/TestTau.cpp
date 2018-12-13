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

    //tau::Generate::GenerateAgent gen("", "");
}

TEST_F(TestTau, TestAgentGen)
{
}
