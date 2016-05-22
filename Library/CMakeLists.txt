cmake_minimum_required(VERSION 3.4)

set(libs Core Executor Network)# NetworkGen)

foreach(lib ${libs})
    def_library(${lib})
endforeach()

# language libs are a bit different
include(${KAI_ROOT_SOURCE_DIR}/Library/Language/CMakeLists.txt)

#$

