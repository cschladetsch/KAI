cmake_minimum_required(VERSION 3.4)

# Linux only set(tests TestExecutor TestCore TestConsole TestPi TestRho TestGamma)
set(tests TestExecutor TestCore TestConsole TestPi TestRho )

include_directories(${KAI_HOME}/Test/Include)

foreach (test ${tests})
    create_test(${test})
endforeach()

# special case the network test, as it needs RakNet
# include(${KAI_HOME}/Test/Tests/TestNetwork/CMakeLists.txt)

# include(${KAI_HOME}/Test/Tests/TestTau/CMakeLists.txt)

