cmake_minimum_required(VERSION 3.4)


set(name TestTau)
project(${name})

set(SOURCE_ROOT ${KAI_HOME}/Test/Tests/${name}/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

add_executable(${name} ${SOURCE_FILES})

link_libraries(${name} Network Core )

target_link_libraries(${name} gtest pthread )#RakNetLibStatic)

