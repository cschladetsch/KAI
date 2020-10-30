cmake_minimum_required(VERSION 3.3)

set(libs Core Executor Network)

foreach(lib ${libs})
    def_library(${lib})
endforeach()

#add_subdirectory(${LIBRARY_HOME}/Network)

# language libs are a bit different
include_directories(${LIBRARY_HOME}/Language/Include)
add_subdirectory(${LIBRARY_HOME}/Language)
