cmake_minimum_required(VERSION 3.4)

set(root ${KAI_HOME}/C/Test/)
include_directories(${KAI_HOME}/C/Include)
set(SOURCE_ROOT ${root}/*.c)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

link_directories(${HOME}/lib /usr/local/lib)
link_libraries(Library_C)

add_executable(Test_C ${SOURCE_FILES})