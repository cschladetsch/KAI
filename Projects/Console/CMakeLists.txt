cmake_minimum_required(VERSION 3.5)
project(Console)

set(SOURCE_FILES Source/Main.cpp)

add_definitions(-DKAI_USE_NAMESPACES -DKAI_NAMESPACE_NAME=kai)
include_directories($ENV{HOME}/Include $ENV{HOME}/dev/KAI/Include ) #usr/local/Cellar/boost/1.60.0_2/include)
set(dir $ENV{HOME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${dir}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${dir}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

link_directories($ENV{HOME}/lib /usr/local/lib)
link_libraries(gtest Executor Library)

add_executable(Console ${SOURCE_FILES})