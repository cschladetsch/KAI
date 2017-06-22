cmake_minimum_required(VERSION 3.7)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/Bin")

set(apps Console NetworkGenerate)# UBUNTU Window)#NetworkPeer Window)

foreach (app ${apps})
    include(${APP_HOME}/${app}/CMakeLists.txt)
endforeach()

