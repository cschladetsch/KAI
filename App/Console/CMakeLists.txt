cmake_minimum_required(VERSION 3.4)

project(Console)

set(SOURCE_ROOT ${KAI_HOME}/Apps/Console/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

link_directories($ENV{HOME}/lib /usr/local/lib)
link_libraries(gtest Executor Library)

add_executable(Console ${SOURCE_FILES} ../../Include/KAI/KAI.h)

