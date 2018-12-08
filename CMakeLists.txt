cmake_minimum_required(VERSION 3.6)

project(KAI)

# setup boost

set(BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE 1)
add_compile_definitions(BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
#set(Boost_DEBUG 1)
set(BOOST_ROOT "c:/Boost/boost_1_66_0")
# for some unknoiwn reason, the following fails, even after setting BOOST_ROOT before calling it. Giving up for now and just hard-coding boost paths :/
# find_package(BOOST COMPONENTS program_options chrono filesystem system)
set(Boost_LIBRARYDIR "c:/Boost/lib")
set(Boost_INCLUDE_DIRS ${BOOST_ROOT})
message(STATUS "Boost IncludeDir: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost Libs: ${Boost_LIBRARYDIR}")

if(!Boost_FOUND)
	message(FATAL_ERROR "We really need boost")
endif()

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARYDIR})

# used tp generate compile options for YouCompleteMe
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")

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

## for X11
#link_directories(/usr/lib/x86_64-linux-gnu)

if (MSVC)
	set(CMAKE_CXX_FLAGS "/std:c++17 /EHsc /D_SILENCE_CXX17_STRSTREAM_DEPRECATION_WARNING")
else()
	set(CMAKE_CXX_FLAGS "-std=c++1y -g -Wno-deprecated -Wno-switch")
endif()

# this was a bit of a nightmare, so it's in a re-usable .cmake file for other projects
include(CMake/IncludeGtest.cmake)

# set common paths for all other projects to use
set(SOURCE_HOME ${KAI_HOME}/Source)
set(APP_HOME ${SOURCE_HOME}/App)
set(LIBRARY_HOME ${SOURCE_HOME}/Library)
set(TEST_HOME ${SOURCE_HOME}/Test)
set(BIN_HOME ${KAI_HOME}/Bin)
set(INCLUDE_HOME ${KAI_HOME}/Include)

include_directories(${INCLUDE_HOME})

set(dir ${KAI_HOME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${dir}/Lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${dir}/Lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/Bin")

link_directories(${HOME}/lib /usr/local/lib)
link_directories(${KAI_HOME}/Lib/Debug)

# make a new library
macro(def_library name)
    project(${name})
    set(SOURCE_ROOT ${LIBRARY_HOME}/${name}/Source/*.cpp)
    file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
    add_library(${name} ${SOURCE_FILES} )
    cotire(${name})
endmacro()

# make a new language library
macro(def_lang_library name)
    project(${name}Lang)
    set(lang_root ${LIBRARY_HOME}/Language/${name})
    include_directories(${lang_root}/Include)
    set(sources ${lang_root}/Source/*.cpp)
    set(headers ${INCLUDE_HOME}/KAI/Language/${name}/*.h)
    file(GLOB_RECURSE src ${sources})
    file(GLOB_RECURSE hdrs ${headers})
    message(STATUS "LANG: ${name} header root at ${headers}")
    message(STATUS "LANG: ${name} source root at ${sources}")
    #    message(STATUS "LANG: ${name} Sources at ${src}")
    #    message(STATUS "LANG: ${name} Headers at ${hdrs}")
    #    message(STATUS "LANG: ${name} Files are ${src} ${hdrs}")
    add_library(${name}Lang ${src} ${hdrs})
    cotire(${name}Lang)
endmacro()

# make a platform-specific library
macro(def_platform_library name)
    project(platform-${name})
    set(SOURCE_ROOT ${LIBRARY_HOME}/Platform/${name}/*.cpp)
    file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
    add_library(platform-${name} SHARED ${SOURCE_FILES})
    cotire(platform-${name})
endmacro()

# make all top-level components
set(TopLevels Library Test App)
foreach(top ${TopLevels})
    include(${SOURCE_HOME}/${top}/CMakeLists.txt)
endforeach()
