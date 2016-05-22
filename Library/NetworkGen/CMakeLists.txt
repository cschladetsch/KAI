cmake_minimum_required(VERSION 3.4)

project(NetworkGen)


include_directories(Include)

set(SOURCE_ROOT Source/*.cpp)

file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

add_library(NetworkGen ${SOURCE_FILES})

