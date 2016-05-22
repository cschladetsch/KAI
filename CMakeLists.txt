cmake_minimum_required(VERSION 3.4)

project(KAI_ROOT)

add_definitions(-DKAI_USE_NAMESPACES)
add_definitions(-DKAI_NAMESPACE_NAME=kai)
add_definitions(-DKAI_DEBUG)
add_definitions(-DKAI_DEBUG_TRACE)
add_definitions(-DKAI_USE_EXCEPTIONS)

# These are defined in KAI/Base/Config.h if KAI_DEBUG is defined
#add_definitions(-DKAI_POINTER_HAS_STORAGEBASE)
#add_definitions(-DKAI_USE_EXCEPTIONS)

set(CMAKE_CXX_FLAGS "-std=c++1y -fpermissive -Wno-deprecated")

set(KAI_HOME $ENV{HOME}/dev/KAI)
set(PROJECTS_HOME ${KAI_HOME}/Projects)
set(TESTS_HOME $ENV{HOME}/Tests)

include_directories($ENV{HOME}/Include ${KAI_HOME}/Include)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    include_directories(/usr/local/Cellar/boost/1.60.0_2/include)
endif()

set(GLOB_RECURSE HEADER_FILES ${KAI_HOME}/Include/KAI/*.h)

set(dir $ENV{HOME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${dir}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${dir}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")

macro(def_lang_library name)
    project(${name}Lang)
    set(lang_root ${KAI_ROOT_SOURCE_DIR}/Library/Language/${name})
    include_directories(${lang_root}/include)
    set(SOURCE_ROOT ${lang_root}/Source/*.cpp)
    file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
    add_library(${name}Lang ${SOURCE_FILES})
endmacro()

macro(def_library name)
    project(${name})
    set(SOURCE_ROOT ${KAI_ROOT_SOURCE_DIR}/Library/${name}/Source/*.cpp)
    file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
    add_library(${name} ${SOURCE_FILES})
endmacro()

macro(create_test name)
    project(${name})

    set(SOURCE_ROOT ${KAI_ROOT_SOURCE_DIR}/Test/${name}/Source/*.cpp)
    file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

    link_directories($ENV{HOME}/lib /usr/local/lib)
    link_libraries(Executor Core Executor PiLang RhoLang)

    add_executable(${name} ${SOURCE_FILES})
    target_link_libraries(${name} gtest pthread)
endmacro()

include(Library/CMakeLists.txt)
include(Test/CMakeLists.txt)
include(App/CMakeLists.txt)

#$
