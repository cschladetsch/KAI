cmake_minimum_required(VERSION 3.6)

set(name Network)
project(${name})

set(source_path ${LIBRARY_HOME}/${name}/Source)
set(header_path ${INCLUDE_HOME}/${name})
file(GLOB_RECURSE sources ${source_path}/*.cpp)
file(GLOB_RECURSE headers ${header_path}/*.h)
link_libraries(Executor Core PiLang RhoLang RakNetLibStatic)
add_library(${name} ${sources} ${headers})
cotire(${name})

