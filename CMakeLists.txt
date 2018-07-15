cmake_minimum_required(VERSION 3.6)

project(KAI)

# setup boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.6 COMPONENTS program_options chrono system)
if(!Boost_FOUND)
	message(FATAL_ERROR "We really need boost")
endif()
include_directories(${Boost_INCLUDE_DIRS})
message(STATUS "Boost IncludeDir: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost LibDir: ${Boost_LIBRARY_DIRS}")
message(STATUS "Boost Libs: ${Boost_LIBRARIES}")

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

add_subdirectory(External)
find_package(GTest REQUIRED)

set(SOURCE_HOME ${KAI_HOME}/Source)
set(APP_HOME ${SOURCE_HOME}/App)
set(LIBRARY_HOME ${SOURCE_HOME}/Library)
set(TEST_HOME ${SOURCE_HOME}/Test)
set(BIN_HOME ${KAI_HOME}/Bin)

include_directories(${KAI_HOME}/Include)

enable_testing()
#find_package(GTest REQUIRED)

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
    set(SOURCE_ROOT ${lang_root}/Source/*.cpp)
    file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
    add_library(${name}Lang ${SOURCE_FILES})
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
