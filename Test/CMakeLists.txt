cmake_minimum_required(VERSION 3.4)

set(tests TestCore TestExecutor TestConsole TestNetwork TestNetworkGen)

foreach (test in {tests})
endforeach()

#set(partner "127.0.0.11")
#
#foreach(test IN {tests})
#    add_network_test({$test} ${partner})
#endforeach()

include(Test/TestCore/CMakeLists.txt)

#$
