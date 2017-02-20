
cmake_minimum_required(VERSION 3.7)

set(name Imgui)
project(${name})
include_directories(${KAI_HOME}/Include/KAI/Imgui)

set(SOURCE_ROOT ${LIBRARY_HOME}/Imgui/Source/*.c*)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

add_library(${name} ${SOURCE_FILES})
