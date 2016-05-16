cmake_minimum_required(VERSION 3.5)

project(TestLibrary)

include($ENV{HOME}/dev/KAI/CMakeLists.common.txt)

file(GLOB SOURCE_FILES $ENV{HOME}/dev/KAI/Tests/Library/Source/*cpp)

link_libraries(Executor Library gtest)

add_executable(TestLibrary ${SOURCE_FILES})
