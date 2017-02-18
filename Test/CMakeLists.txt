# Build all tests for the KAI framework

cmake_minimum_required(VERSION 3.4)
project(KaiTests)
set(TEST_ROOT ${KAI_HOME}/Test)
set(TEST_SRC_ROOT ${TEST_ROOT}/Source/*.cpp)
file(GLOB_RECURSE TEST_SRC_FILES ${TEST_SRC_ROOT})
link_libraries(Executor Core PiLang RhoLang TauLang)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/Test/Bin")
add_executable(KaiTests ${TEST_SRC_FILES})
target_link_libraries(KaiTests gtest pthread)
