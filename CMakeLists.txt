cmake_minimum_required(VERSION 3.6)

project(KAI)

option(KAI_BUILD_RAKNET	"Build RakNet" OFF)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")

include(cotire)
include(FindBoost)
include(IncludeGtest)

# used tp generate compile options for YouCompleteMe
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(-DKAI_USE_NAMESPACES)
add_definitions(-DKAI_NAMESPACE_NAME=kai)
add_definitions(-DKAI_USE_EXCEPTIONS)
add_definitions(-DKAI_BOOST_UNORDERED_REGISTRY)
add_definitions(-DKAI_DEBUG)
#add_definitions(-DKAI_DEBUG_TRACE)
#add_definitions(-DKAI_TRACE_FILE_LOCATION)
#add_definitions(-DKAI_DEBUG_REGISTRY)

if (MSVC)
    set(CMAKE_CXX_FLAGS "/std:c++17 /EHsc /D_SILENCE_CXX17_STRSTREAM_DEPRECATION_WARNING")
else()
    set(CMAKE_CXX_FLAGS "-std=c++1y -g -Wno-deprecated -Wno-switch")
endif()

set(HOME $ENV{HOME})
set(KAI_HOME ${CMAKE_SOURCE_DIR})

# set common paths for all other projects to use
set(SOURCE_HOME ${KAI_HOME}/Source)
set(APP_HOME ${SOURCE_HOME}/App)
set(LIBRARY_HOME ${SOURCE_HOME}/Library)
set(TEST_HOME ${SOURCE_HOME}/Test)
set(BIN_HOME ${KAI_HOME}/Bin)
set(INCLUDE_HOME ${KAI_HOME}/Include)
set(EXTERNAL ${KAI_HOME}/Ext)

set(dir ${KAI_HOME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${dir}/Lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${dir}/Lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/Bin")

include_directories(${INCLUDE_HOME})

link_directories(${HOME}/lib /usr/local/lib)
link_directories(${KAI_HOME}/Lib/Debug)

# Add RakNet
set(RAKNET_GENERATE_INCLUDE_ONLY_DIR ON)
set(RAKNET_ROOT ${EXTERNAL}/RakNet)
if(KAI_BUILD_RAKNET)
	add_subdirectory(${RAKNET_ROOT})
endif()
include_directories(${RAKNET_ROOT}/include)

# Add rang
include_directories(${EXTERNAL}/rang/include)

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
    #message(STATUS "LANG: ${name} header root at ${headers}")
    #message(STATUS "LANG: ${name} source root at ${sources}")
    #message(STATUS "LANG: ${name} Sources at ${src}")
    #message(STATUS "LANG: ${name} Headers at ${hdrs}")
    #message(STATUS "LANG: ${name} Files are ${src} ${hdrs}")
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
set(TopLevels Library App Test)
foreach(top ${TopLevels})
    include(${SOURCE_HOME}/${top}/CMakeLists.txt)
endforeach()

