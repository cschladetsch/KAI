
cmake_minimum_required(VERSION 3.4)

set(name Imgui)
project(${name})
set(SOURCE_ROOT ${LIBRARY_HOME}/Imgui/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
include_directories(${KAI_HOME}/Include/KAI/Imgui)
add_library(${name} ${SOURCE_FILES})
