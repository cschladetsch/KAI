cmake_minimum_required(VERSION 3.4)
project(Network)

set(SOURCE_ROOT ${KAI_HOME}/Projects/Network/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

add_library(Network ${SOURCE_FILES})

