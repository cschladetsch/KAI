cmake_minimum_required(VERSION 3.4)

set(tests TestExecutor TestCore TestConsole TestPi TestRho TestRxCpp TestGamma TestNetworkGen)#TestNetwork TestNetworkGen)

foreach (test ${tests})
    create_test(${test})
endforeach()

