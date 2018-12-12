cmake_minimum_required(VERSION 3.6)

set(name Network)
project(${name})

set(SOURCE_ROOT Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
link_libraries(Executor Core PiLang RhoLang RakNetLibStatic)
add_library(${name} ${SOURCE_FILES})
cotire(${name})

