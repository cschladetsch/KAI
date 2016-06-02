cmake_minimum_required(VERSION 3.4)

project(KAI)

add_definitions(-DKAI_USE_NAMESPACES)
add_definitions(-DKAI_NAMESPACE_NAME=kai)
add_definitions(-DKAI_DEBUG)
add_definitions(-DKAI_DEBUG_TRACE)
add_definitions(-DKAI_USE_EXCEPTIONS)
add_definitions(-DKAI_TRACE_FILE_LOCATION)
#add_definitions(-DKAI_TRACE_VERBOSE)

set(CMAKE_CXX_FLAGS "-std=c++1y -g -Wno-deprecated")
set(HOME $ENV{HOME})
set(KAI_HOME ${HOME}/dev/KAI)
set(LIBRARY_HOME ${KAI_HOME}/Library)
set(TESTS_HOME ${KAI_HOME}/Tests)

include_directories(${KAI_HOME}/Include)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    include_directories(/usr/local/Cellar/boost/1.60.0_2/include)
    include_directories(/usr/local/)
endif()

set(dir ${HOME}/kai)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${dir}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${dir}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")

link_directories(${HOME}/lib /usr/local/lib)

macro(def_lang_library name)
    project(${name}Lang)
    set(lang_root ${KAI_HOME}/Library/Language/${name})
    include_directories(${lang_root}/include)
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

macro(create_test name)
    project(${name})
    set(SOURCE_ROOT ${KAI_HOME}/Test/${name}/Source/*.cpp)
    file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
    link_directories(${HOME}/lib /usr/local/lib)
    link_libraries(Executor Core Executor PiLang RhoLang NetworkGen)
    add_executable(${name} ${SOURCE_FILES})
    target_link_libraries(${name} gtest pthread RakNetLibStatic)
endmacro()

include(Library/CMakeLists.txt)
include(Test/CMakeLists.txt)
include(App/CMakeLists.txt)

#$
