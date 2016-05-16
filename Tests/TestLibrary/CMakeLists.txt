cmake_minimum_required(VERSION 3.5)

project(TestLibrary)

include ($ENV{HOME}/dev/KAI/CMakeLists.common.txt)

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp)

link_libraries(gtest Executor Library)

add_executable(TestLibrary ${SOURCE_FILES})
