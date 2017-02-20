cmake_minimum_required(VERSION 3.4)

set(name ImGuiConsole)
project(${name})

set(SOURCE_ROOT ${KAI_HOME}/App/${name}/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

link_libraries(Executor Core PiLang RhoLang Imgui)

add_executable(${name} ${SOURCE_FILES})
cotire(${name})
