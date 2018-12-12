cmake_minimum_required(VERSION 3.3)

set(libs Core Executor)#Network)

foreach(lib ${libs})
    def_library(${lib})
endforeach()

message(STATUS ---- starting NETWORK)
add_subdirectory(${LIBRARY_HOME}/Network)
message(STATUS ---- ending NETWORK)

# language libs are a bit different
include_directories(${LIBRARY_HOME}/Language/Include)
add_subdirectory(${LIBRARY_HOME}/Language)

# CJS UBUNTU include(${LIBRARY_HOME}/ImGui/CMakeLists.txt)
