cmake_minimum_required(VERSION 3.6)

project(KAI)

# sorry
set(BOOST_ROOT C:/Users/christian/external/boost_1_67_0)

# used tp generate compile options for YouCompleteMe
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
# see https://github.com/sakra/cotire
include(cotire)

add_definitions(-DKAI_USE_NAMESPACES)
add_definitions(-DKAI_NAMESPACE_NAME=kai)
add_definitions(-DKAI_DEBUG)
add_definitions(-DKAI_DEBUG_TRACE)
add_definitions(-DKAI_USE_EXCEPTIONS)
add_definitions(-DKAI_BOOST_UNORDERED_REGISTRY)
# add_definitions(-DKAI_TRACE_FILE_LOCATION)
# add_definitions(-DKAI_TRACE_VERBOSE)
# add_definitions(-DKAI_DEBUG_REGISTRY)

set(HOME $ENV{HOME})
set(KAI_HOME ${CMAKE_SOURCE_DIR})

# RakNet
set(RAKNET_DIR ${HOME}/External/RakNet)
include(${KAI_HOME}/CMake/FindRakNet.cmake)

## for X11
#link_directories(/usr/lib/x86_64-linux-gnu)

if (MSVC)
	# Visual Studio 2017
	set(CMAKE_CXX_FLAGS "/std:c++17 /EHsc /D_SILENCE_CXX17_STRSTREAM_DEPRECATION_WARNING")
else()
	# Clang (and I think GCC)
	set(CMAKE_CXX_FLAGS "-std=c++1y -g -Wno-deprecated -Wno-switch")
endif()

set(SOURCE_HOME ${KAI_HOME}/Source)
set(APP_HOME ${SOURCE_HOME}/App)
set(LIBRARY_HOME ${SOURCE_HOME}/Library)
set(TEST_HOME ${SOURCE_HOME}/Test)
set(BIN_HOME ${KAI_HOME}/Bin)

include_directories(${KAI_HOME}/Include)

include(${KAI_HOME}/External/CMakeLists.txt)

set(dir ${KAI_HOME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${dir}/Lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${dir}/Lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/Bin")

link_directories(${HOME}/lib /usr/local/lib)
link_directories(${KAI_HOME}/Lib/Debug)
link_directories(${BOOST_ROOT}/lib)

macro(def_lang_library name)
    project(${name}Lang)
    set(lang_root ${LIBRARY_HOME}/Language/${name})
    include_directories(${lang_root}/Include)
    set(SOURCE_ROOT ${lang_root}/Source/*.cpp)
    file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
    add_library(${name}Lang ${SOURCE_FILES})
    cotire(${name}Lang)
endmacro()

macro(def_library name)
    project(${name})
    set(SOURCE_ROOT ${LIBRARY_HOME}/${name}/Source/*.cpp)
    file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
    add_library(${name} ${SOURCE_FILES} )
    cotire(${name})
endmacro()

macro(def_platform_library name)
    project(platform-${name})
    set(SOURCE_ROOT ${LIBRARY_HOME}/Platform/${name}/*.cpp)
    file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
    add_library(platform-${name} SHARED ${SOURCE_FILES})
    cotire(platform-${name})
endmacro()

set(TopLevels Library Test App)
foreach(top ${TopLevels})
    include(${SOURCE_HOME}/${top}/CMakeLists.txt)
endforeach()

# TODO
# include(Library/Platform/CMakeLists.txt)
