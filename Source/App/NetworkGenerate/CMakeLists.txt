# cmake_minimum_required(VERSION 3.7)

set(name NetworkGenerate)
project(${name})

find_package(Boost 1.57.0 COMPONENTS program_options system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

#message(WARNING *** BD=${Boost_INCLUDE_DIR})

include_directories(${name}/Include)
set(SOURCE_ROOT ${SOURCE_HOME}/App/${name}/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
link_libraries(TauLang Executor Core )
add_executable(${name} ${SOURCE_FILES})

target_link_libraries(${name} boost_program_options boost_system)

