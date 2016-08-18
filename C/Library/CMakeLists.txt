cmake_minimum_required(VERSION 3.4)

set(root ${KAI_HOME}/C/Library/)
include_directories(${KAI_HOME}/C/Include)
set(SOURCE_ROOT ${root}/*.c)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
add_library(Library_C ${SOURCE_FILES})

