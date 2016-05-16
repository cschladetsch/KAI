cmake_minimum_required(VERSION 3.5)
project(Console)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

set(SOURCE_FILES
    Source/Main.cpp)

add_definitions(-DKAI_USE_NAMESPACES -DKAI_NAMESPACE_NAME=kai)

include_directories(/Users/christian/Include /Users/christian/dev/KAI/Include /usr/local/Cellar/boost/1.60.0_2/include)

target_link_libraries( Executor Library)

add_executable(Console ${SOURCE_FILES})
