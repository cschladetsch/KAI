
cmake_minimum_required(VERSION 3.7)

set(name ImGui)
project(${name})
include_directories(${KAI_HOME}/Include/KAI/ImGui)

set(SOURCE_ROOT ${LIBRARY_HOME}/${name}/Source/*.c*)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

add_library(${name} ${SOURCE_FILES})
