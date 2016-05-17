cmake_minimum_required(VERSION 3.4)

project(KAI_ROOT)

add_definitions(-DKAI_USE_NAMESPACES)
add_definitions(-DKAI_NAMESPACE_NAME=kai)
add_definitions(-DKAI_DEBUG)
add_definitions(-DKAI_DEBUG_TRACE)
#add_definitions(-DKAI_POINTER_HAS_STORAGEBASE)
#add_definitions(-DKAI_USE_EXCEPTIONS)

set(CMAKE_CXX_FLAGS "-std=c++1y -fpermissive -Wno-deprecated")

set(KAI_HOME $ENV{HOME}/dev/KAI)
set(PROJECTS_HOME ${KAI_HOME}/Projects)
set(TESTS_HOME $ENV{HOME}/Tests)

include_directories($ENV{HOME}/Include ${KAI_HOME}/Include)
# only needed for OSX
#include_directories(/usr/local/Cellar/boost/1.60.0_2/include)

set(GLOB_RECURSE HEADER_FILES ${KAI_HOME}/Include/KAI/*.h)

set(dir $ENV{HOME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${dir}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${dir}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -fpermissive")

include(Projects/Library/CMakeLists.txt)
include(Projects/Executor/CMakeLists.txt)
include(Projects/Console/CMakeLists.txt)
#include(Tests/TestLibary/CMakeLists.txt)
