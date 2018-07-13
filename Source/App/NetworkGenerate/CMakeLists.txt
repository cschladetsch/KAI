# cmake_minimum_required(VERSION 3.7)

set(name NetworkGenerate)
project(${name})

set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost COMPONENTS program_options system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

include_directories(${name}/Include)
set(SOURCE_ROOT ${SOURCE_HOME}/App/${name}/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
link_libraries(TauLang Executor Core )
add_executable(${name} ${SOURCE_FILES})

target_link_libraries(${name} ${Boost_LIBRARIES})

