cmake_minimum_required(VERSION 3.4)

project(TauGen)

set(SOURCE_ROOT ${KAI_ROOT_SOURCE_DIR}/App/TauGen/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

link_directories($ENV{HOME}/lib /usr/local/lib)
#link_libraries(Executor Core PiLang RhoLang)

add_executable(TauGen ${SOURCE_FILES})

