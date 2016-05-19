cmake_minimum_required(VERSION 3.4)
project(Executor)

set(SOURCE_ROOT ${KAI_HOME}/Libraries/Executor/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

add_library(Executor ${SOURCE_FILES} ../../Include/KAI/KAI.h)
