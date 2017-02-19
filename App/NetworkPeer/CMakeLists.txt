cmake_minimum_required(VERSION 3.4)

project(Peer)

set(SOURCE_ROOT ${KAI_HOME}/App/NetworkPeer/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

link_libraries(Executor Core PiLang RhoLang RakNetLibStatic)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/Bin")
add_executable(Peer ${SOURCE_FILES})
