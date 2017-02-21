# Build all tests for the KAI framework

cmake_minimum_required(VERSION 3.7)
project(KaiTests)
set(TEST_SRC_ROOT ${TEST_HOME}/Source/*.cpp)
file(GLOB_RECURSE TEST_SRC_FILES ${TEST_SRC_ROOT})
include_directories(${TEST_HOME}/Include)
link_libraries(Executor Core PiLang RhoLang TauLang)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_HOME}/Test")
add_executable(KaiTests ${TEST_SRC_FILES})
target_link_libraries(KaiTests gtest pthread)
