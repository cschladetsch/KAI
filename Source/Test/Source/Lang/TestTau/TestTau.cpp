#include "TestLangCommon.h"


using namespace kai;
using namespace std;

struct TestTau : TestLangCommon
{
};

TEST_F(TestTau, TestProxyGen)
{
    auto tauScript = LoadScriptText("Tau2.tau");

}

TEST_F(TestTau, TestAgentGen)
{
}
