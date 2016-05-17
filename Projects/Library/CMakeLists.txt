cmake_minimum_required(VERSION 3.4)

project(Library)

set(SOURCE_ROOT ${KAI_HOME}/Projects/Library/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

add_library(Library ${SOURCE_FILES})

