# cmake_minimum_required(VERSION 3.7)

set(name NetworkGenerate)
project(${name})

include_directories(${name}/Include)
set(SOURCE_ROOT ${SOURCE_HOME}/App/${name}/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
link_libraries(TauLang Executor Core )
add_executable(${name} ${SOURCE_FILES})

target_link_libraries(${name} ${Boost_LIBRARIES})

