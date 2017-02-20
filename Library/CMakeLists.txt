cmake_minimum_required(VERSION 3.4)

set(libs Core Executor Network)

foreach(lib ${libs})
    def_library(${lib})
endforeach()

# language libs are a bit different
include_directories(${KAT_HOME}/Library/Language/Include)
include(${KAI_HOME}/Library/Language/CMakeLists.txt)

include(${LIBRARY_HOME}/Imgui/CMakeLists.txt)

#$

