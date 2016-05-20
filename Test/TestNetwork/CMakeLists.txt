cmake_minimum_required(VERSION 3.4)

set(tests Pipe String Tcp)

macro(add_network_test name)
endmacro()

foreach(test in ${tests)
    add_network_test(${test})
endforeach()

