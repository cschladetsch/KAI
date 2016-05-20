cmake_minimum_required(VERSION 3.4)

set(tests TestCore TestExecutor TestConsole TestNetwork TestNetworkGen)

macro(add_netork_test name parter)
    project(${name})
    set(SOURCE_ROOT ${KAI_ROOT_SOURCE_DIR}/Test/Test${name}/Source/*.cpp)
    file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
    add_library(${name} ${SOURCE_FILES})
endmacro()

#set(partner "127.0.0.11")
#
#foreach(test IN {tests})
#    add_network_test({$test} ${partner})
#endforeach()



#$
