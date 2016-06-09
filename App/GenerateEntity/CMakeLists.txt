cmake_minimum_required(VERSION 3.4)

set(name GenerateEntity)
project(GenerateEntity)
include_directories(${name}/Include)
set(SOURCE_ROOT ${KAI_HOME}/App/${name}/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
link_libraries(TauLang Core)
add_executable(${name} ${SOURCE_FILES})

