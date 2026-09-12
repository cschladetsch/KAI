#include <gtest/gtest.h>

#include <KAI/Console/Console.h>

#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

TEST(ContinuationMobilityDemoScriptTest, ScriptExecutesSuccessfully) {
    kai::Console console;
    console.SetLanguage(kai::Language::Rho);

    // KAI_LANG_ROOT is injected by Test/Language/CMakeLists.txt as the
    // absolute path to this test's own source directory
    // (Test/Language/TestRho), where a copy of ContinuationMobilityDemo.rho
    // is checked in alongside this file. The old hardcoded relative path
    // ("Demo/ContinuationMobilityDemo/...") only resolved when ctest was run
    // from the repo root, since it depends on the process's current working
    // directory rather than anything fixed at compile time.
    const fs::path scriptPath =
        fs::path(KAI_STRINGISE(KAI_LANG_ROOT)) / "ContinuationMobilityDemo.rho";
    ASSERT_TRUE(fs::exists(scriptPath))
        << "Script not found at " << scriptPath.string();

    std::ifstream file(scriptPath);
    ASSERT_TRUE(file.is_open());

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string script = buffer.str();

    EXPECT_NO_THROW(console.Execute(script.c_str(), kai::Structure::Program));
}
