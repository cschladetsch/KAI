cmake_minimum_required(VERSION 3.4)

project(Chat)

set(SOURCE_ROOT ${KAI_HOME}/App/NetworkChat)

link_libraries(Executor Core PiLang RhoLang RakNetLibStatic)

add_executable(Server ${SOURCE_ROOT}/Server.cpp)
add_executable(Client ${SOURCE_ROOT}/Client.cpp)
