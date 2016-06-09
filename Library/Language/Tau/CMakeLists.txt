cmake_minimum_required(VERSION 3.4)
set(name Tau)
project(${name})
set(NETWORK_GEN_HOME ${LIBRARY_HOME}/${Tau})
include_directories(${NETWORK_GEN_HOME}/Include)
message(WARNING --LIB_HOME:${LIBRARY_HOME} --  NGH:${NETWORK_GEN_HOME})
set(SOURCE_ROOT ${NETWORK_GEN_HOME}/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
add_library(${name} ${SOURCE_FILES})

#$
