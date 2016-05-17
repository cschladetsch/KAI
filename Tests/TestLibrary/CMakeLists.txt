cmake_minimum_required(VERSION 3.4)

project(TestLibrary)

set(SOURCE_ROOT ${KAI_HOME}/Tests/TestLibrary/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

add_executable(TestLibrary ${SOURCE_FILES})

link_libraries(TestLibrary gtest pthread)
target_link_libraries(TestLibrary Executor Library)


