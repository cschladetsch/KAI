cmake_minimum_required(VERSION 3.6)

project(NetworkPeer)

set(SOURCE_ROOT ${SOURCE_HOME}/App/NetworkPeer/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

link_libraries(Executor Core PiLang RhoLang RakNetLibStatic)
include_directories(${RAKNET_ROOT}/Source)
add_executable(NetworkPeer ${SOURCE_FILES})
cotire(NetworkPeer)
