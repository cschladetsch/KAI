cmake_minimum_required(VERSION 3.4)
project(NetworkGen)

set(SOURCE_ROOT ${KAI_HOME}/Libraries/NetworkGen/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

add_library(NetworkGen ${SOURCE_FILES} ../../Include/KAI/KAI.h)

