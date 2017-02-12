cmake_minimum_required(VERSION 3.4)

project(KAI)

add_definitions(-DKAI_USE_NAMESPACES)
add_definitions(-DKAI_NAMESPACE_NAME=kai)
add_definitions(-DKAI_DEBUG)
add_definitions(-DKAI_DEBUG_TRACE)
add_definitions(-DKAI_USE_EXCEPTIONS)
add_definitions(-DKAI_TRACE_FILE_LOCATION)
#add_definitions(-DKAI_TRACE_VERBOSE)

set(HOME $ENV{HOME})

# THESE ARE HARD-CODED AND WILL HAVE TO BE CHANGED FOR YOUR INSTALLATION
# Source location.
set(KAI_HOME ${HOME}/Local/KAI)
# RakNet
#include_directories(${HOME}/External/RakNet/include)
## for X11
#link_directories(/usr/lib/x86_64-linux-gnu)
## END HARD-CODED

set(CMAKE_CXX_FLAGS "-std=c++1y -g -Wno-deprecated -Wno-switch")
set(LIBRARY_HOME ${KAI_HOME}/Library)
set(TESTS_HOME ${KAI_HOME}/Tests)

include_directories(${KAI_HOME}/Include)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    include_directories(/usr/local/Cellar/boost/1.60.0_2/include)
    include_directories(/usr/local/)
endif()

set(dir ${KAI_HOME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${dir}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${dir}/lib")

# FLTK

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")

link_directories(${HOME}/lib /usr/local/lib)

macro(def_lang_library name)
    project(${name}Lang)
    set(lang_root ${KAI_HOME}/Library/Language/${name})
    include_directories(${lang_root}/Include)
    set(SOURCE_ROOT ${lang_root}/Source/*.cpp)
    file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
    add_library(${name}Lang ${SOURCE_FILES})
endmacro()

macro(def_library name)
    project(${name})
    set(SOURCE_ROOT ${KAI_HOME}/Library/${name}/Source/*.cpp)
    file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
    add_library(${name} ${SOURCE_FILES})
endmacro()

macro(def_platform_library name)
    project(platform-${name})
    set(SOURCE_ROOT ${KAI_HOME}/Platform/${name}/*.cpp)
    file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
    add_library(platform-${name} SHARED ${SOURCE_FILES})
endmacro()

macro(create_test name)
    project(${name})
    include_directories(${KAI_HOME}/Test/Include)

    set(TEST_ROOT ${KAI_HOME}/Test)
    set(TEST_SRC_ROOT ${TEST_ROOT}/Tests/${name}/Source/*.cpp)

    file(GLOB_RECURSE TEST_SRC_FILES ${TEST_SRC_ROOT} ${TEST_ROOT}/Source)

    link_libraries(Executor Core PiLang RhoLang)
    add_executable(${name} ${TEST_SRC_FILES})
    target_link_libraries(${name} gtest pthread)
endmacro()

include(CMakeToolsHelpers OPTIONAL)

include(Library/CMakeLists.txt)
include(Test/CMakeLists.txt)
include(App/CMakeLists.txt)

# include(Platform/CMakeLists.txt)


#$
