cmake_minimum_required(VERSION 3.4)

project(TestConsole)

set(SOURCE_ROOT ${KAI_ROOT_SOURCE_DIR}/Test/TestConsole/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

add_executable(TestConsole ${SOURCE_FILES})

target_link_libraries(TestConsole gtest pthread)
link_libraries(TestConsole Executor Core)


