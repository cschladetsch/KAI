cmake_minimum_required(VERSION 3.4)

project(NetworkGen)

set(SOURCE_ROOT ${KAI_HOME}/Tests/TestExecutor/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

add_executable(NetworkGen ${SOURCE_FILES})

target_link_libraries(NetworkGen gtest pthread)
link_libraries(NetworkGen Library Executor Network)


