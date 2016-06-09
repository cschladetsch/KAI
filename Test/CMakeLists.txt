cmake_minimum_required(VERSION 3.4)

set(tests TestExecutor TestCore TestConsole TestPi TestRho TestGamma)

foreach (test ${tests})
    create_test(${test})
endforeach()

# special case the network test, as it needs RakNet
include(${KAI_HOME}/Test/TestNetwork/CMakeLists.txt)

include(${KAI_HOME}/Test/TestTau/CMakeLists.txt)

