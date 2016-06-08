cmake_minimum_required(VERSION 3.4)

set(libs Core Executor Network)

foreach(lib ${libs})
    def_library(${lib})
endforeach()

# language libs are a bit different
# this library contains the two languages, Pi and Rho (Rho translates to Pi)
include(${KAI_HOME}/Library/Language/CMakeLists.txt)

#$

