cmake_minimum_required(VERSION 3.4)

project(TestNetworkGen)

set(SOURCE_ROOT ${KAI_HOME}/Tests/TestNetwtorkGen/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

add_executable(TestNetworkGen ${SOURCE_FILES})

target_link_libraries(TestNetworkGen gtest pthread)
link_libraries(TestNetworkGen Library Executor)


