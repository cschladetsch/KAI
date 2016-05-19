cmake_minimum_required(VERSION 3.4)

project(TestExecutor)

set(SOURCE_ROOT ${KAI_HOME}/Tests/TestExecutor/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

add_executable(TestExecutor ${SOURCE_FILES})

target_link_libraries(TestExecutor gtest pthread)
link_libraries(TestExecutor Library Executor)


