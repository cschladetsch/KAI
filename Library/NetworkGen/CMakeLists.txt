cmake_minimum_required(VERSION 3.4)

project(NetworkGen)

set(NETWORK_GEN_HOME ${LIBRARY_HOME}/NetworkGen)
include_directories(${NETWORK_GEN_HOME}/Include)

set(SOURCE_ROOT ${NETWORK_GEN_HOME}/Source/*.cpp)

file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

add_library(NetworkGen ${SOURCE_FILES})

#$
