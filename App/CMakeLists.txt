cmake_minimum_required(VERSION 3.4)

set(apps Console Window)

foreach (app ${apps})
    include(${KAI_HOME}/App/${app}/CMakeLists.txt)
endforeach()

