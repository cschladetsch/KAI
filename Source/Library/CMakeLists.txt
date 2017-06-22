cmake_minimum_required(VERSION 3.7)

set(libs Core Executor)#Network)

foreach(lib ${libs})
    def_library(${lib})
endforeach()

# language libs are a bit different
include_directories(${LIBRARY_HOME}/Language/Include)
include(${LIBRARY_HOME}/Language/CMakeLists.txt)

# CJS UBUNTU include(${LIBRARY_HOME}/ImGui/CMakeLists.txt)
