# Build all tests for the KAI framework

cmake_minimum_required(VERSION 3.6)

set(name KaiTest)
project(${name})
set(source_path ${TEST_HOME}/Source)
set(header_path ${TEST_HOME}/Include)
set(script_path ${TEST_HOME}/Scripts)
file(GLOB_RECURSE sources ${source_path}/*.cpp)
file(GLOB_RECURSE headers ${header_path}/*.h)
file(GLOB_RECURSE pi_scripts ${script_path}/*.pi)
file(GLOB_RECURSE rho_scripts ${script_path}/*.rho)
file(GLOB_RECURSE tau_scripts ${script_path}/*.tau)
include_directories(${header_path})
link_libraries(Executor Core PiLang RhoLang TauLang ${GTEST_BOTH_LIBRARIES} ${Boost_LIBRARIES})# HlslLang)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_HOME}/Test")
add_executable(${name} ${sources} ${headers} ${pi_scripts} ${rho_scripts} ${tau_scripts})
cotire(${name})

