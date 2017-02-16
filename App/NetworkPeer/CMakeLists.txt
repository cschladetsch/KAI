cmake_minimum_required(VERSION 3.4)

project(Peer)

set(SOURCE_ROOT ${KAI_HOME}/App/NetworkPeer)

link_libraries(Executor Core PiLang RhoLang RakNetLibStatic)

add_executable(Peer ${SOURCE_ROOT}/Peer.cpp)
