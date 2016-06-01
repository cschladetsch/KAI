cmake_minimum_required(VERSION 3.4)

project(TestNetwork)

set(SOURCE_ROOT ${KAI_HOME}/Test/TestNetwork/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

add_executable(TestNetwork ${SOURCE_FILES})

target_link_libraries(TestNetwork gtest pthread RakNetLibStatic)


