cmake_minimum_required(VERSION 3.4)

set(tests TestCore TestRegistry TestExecutor TestConsole TestString)# TestPi TestRho )

include_directories(${KAI_HOME}/Test/Include)

foreach (test ${tests})
    create_test(${test})
endforeach()

# special case the network test, as it needs RakNet
# include(${KAI_HOME}/Test/Tests/TestNetwork/CMakeLists.txt)

# include(${KAI_HOME}/Test/Tests/TestTau/CMakeLists.txt)

#include(${HOME}/Local/fl)
