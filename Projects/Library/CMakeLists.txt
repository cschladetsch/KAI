cmake_minimum_required(VERSION 3.5)
project(Core)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -I/home/christian/dev/KAI/Include")

file(GLOB_RECURSE SOURCE_FILES Source/*.cpp)
#file( SOURCE_FILES Source/Class.cpp)

add_library(Core ${SOURCE_FILES})


