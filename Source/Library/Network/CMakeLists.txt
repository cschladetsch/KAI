cmake_minimum_required(VERSION 3.6)

set(name Network)
project(${name})

set(SOURCE_ROOT Source/*.cpp)
set(HEADER_ROOT ${INCLUDE_ROOT}/Network/*.h)
file(GLOB_RECURSE srcs ${SOURCE_ROOT})
file(GLOB_RECURSE headers ${HEADER_ROOT})
link_libraries(Executor Core PiLang RhoLang RakNetLibStatic)
add_library(${name} ${srcs} ${headers})
cotire(${name})

