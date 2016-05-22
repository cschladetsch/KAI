cmake_minimum_required(VERSION 3.4)

project(TestCore)

set(SOURCE_ROOT ${KAI_ROOT_SOURCE_DIR}/Test/TestCore/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

link_directories($ENV{HOME}/lib /usr/local/lib)
link_libraries(Executor Core Executor PiLang RhoLang)

add_executable(TestCore ${SOURCE_FILES})
target_link_libraries(TestCore gtest pthread)


