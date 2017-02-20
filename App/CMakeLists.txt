cmake_minimum_required(VERSION 3.4)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/Bin")

set(apps Console NetworkGenerate RakNetworkChat NetworkPeer GuiConsole)

foreach (app ${apps})
    include(${KAI_HOME}/App/${app}/CMakeLists.txt)
endforeach()

