cmake_minimum_required(VERSION 3.4)

# Ubuntu only set(libs Core Executor Network)
set(libs Core Executor)

include_directories(${KAT_HOME}/Library/Language/Include)

foreach(lib ${libs})
    def_library(${lib})
endforeach()

# language libs are a bit different
include(${KAI_HOME}/Library/Language/CMakeLists.txt)

#$

