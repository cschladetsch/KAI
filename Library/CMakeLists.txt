cmake_minimum_required(VERSION 3.4)

set(libs Core Executor Network)# NetworkGen)

foreach(lib ${libs})
    def_library(${lib})
endforeach()

# language libs are a bit different
# this library contains the two languages, Pi and Rho (Rho translates to Pi)
include(${KAI_ROOT_SOURCE_DIR}/Library/Language/CMakeLists.txt)

# The NetwrorkGen system uses the Tau language, which share common core
# with both Pi and Rho, but is not executed: rather, it it is an
# Interface Definition Languagee
include(${KAI_ROOT_SOURCE_DIR}/Library/NetworkGen/CMakeLists.txt)

#$

