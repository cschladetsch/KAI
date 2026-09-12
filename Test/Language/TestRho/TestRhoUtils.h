#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace kai {
namespace test {

// Helper function to load a Rho script from file.
//
// KAI_SCRIPT_ROOT is injected by Test/Language/CMakeLists.txt as the
// absolute path to TestRho/Scripts/ (target_compile_definitions there). The
// old hardcoded "Test/Language/TestRho/Scripts/" path was relative to
// whatever directory ctest happened to be invoked from, so it silently
// failed to open (and every RunScriptAndExpect() call in
// RhoScriptBasedTests.cpp failed with it) unless ctest was run from the
// repo root.
inline std::string LoadRhoScript(const std::string& filename) {
    const std::filesystem::path path =
        std::filesystem::path(KAI_STRINGISE(KAI_SCRIPT_ROOT)) / filename;
    std::ifstream file(path);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open script file: " +
                                  path.string());
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

}  // namespace test
}  // namespace kai