cmake_minimum_required(VERSION 3.4)

set(Libs Core Executor Language Network NetworkGen)

def_library(Core)
def_library(Executor)

include(${KAI_ROOT_SOURCE_DIR}/Library/Language/CMakeLists.txt)

#$

