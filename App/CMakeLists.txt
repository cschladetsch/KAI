cmake_minimum_required(VERSION 3.4)

# Ubuntu only set(apps Window Console Generate)
set(apps Console)

foreach (app ${apps})
    include(${KAI_HOME}/App/${app}/CMakeLists.txt)
endforeach()

