cmake_minimum_required(VERSION 3.4)

set(name TestNetwork)
project(${name})

set(SOURCE_ROOT ${KAI_HOME}/Test/${name}/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

link_libraries(${name} Network Core)

add_executable(${name} ${SOURCE_FILES})

target_link_libraries(${name} gtest pthread )#RakNetLibStatic)


