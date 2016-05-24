cmake_minimum_required(VERSION 3.4)

project(RxCpp)

set(SOURCES ${KAI_ROOT_SOURCE_DIR}/App/RxCpp/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCES})

add_executable(RxCpp ${SOURCE_FILES})
