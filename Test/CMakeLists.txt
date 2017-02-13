cmake_minimum_required(VERSION 3.4)

project(KaiTests)

set(TEST_ROOT ${KAI_HOME}/Test)
set(TEST_SRC_ROOT ${TEST_ROOT}/Source/*.cpp)
file(GLOB_RECURSE TEST_SRC_FILES ${TEST_SRC_ROOT})
link_libraries(Executor Core PiLang RhoLang)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/Bin/Test")
add_executable(KaiTests ${TEST_SRC_FILES})
target_link_libraries(KaiTests gtest pthread)

# special case the network test, as it needs RakNet
# include(${KAI_HOME}/Test/Tests/TestNetwork/CMakeLists.txt)

# include(${KAI_HOME}/Test/Tests/TestTau/CMakeLists.txt)

#include(${HOME}/Local/fl)
