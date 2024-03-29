cmake_minimum_required(VERSION 3.10)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_HOME}/Test")
set(core_libs Executor Core PiLang RhoLang TauLang)

macro(build_lang_test lang)
endmacro()
message(STATUS "GOOGLETESTDIR - ${CMAKE_BINARY_DIR}/googletest")
#include(${CMAKE_BINARY_DIR}/_deps/googletest-src/googletest/include)

set(langs Tau Rho Pi)
foreach (lang ${langs})
    project(Test${lang})
    set(testName Test${lang})
    message(STATUS "--- building unit tests for ${testName} ---")
    set(home ${TEST_HOME}/Language/Test${lang})
    set(source_path ${home})
    set(header_path ${home}/Include)
    set(script_path ${TEST_HOME}/Language/Test${lang}/Scripts/)
    string(TOLOWER ${lang} ext)
    file(GLOB_RECURSE sources ${source_path}/*.cpp)
    list(APPEND sources ${TEST_HOME}/Language/TestLangCommon.cpp)
    list(APPEND sources ${TEST_HOME}/Common/TestCommon.cpp)
    list(APPEND sources ${TEST_HOME}/Common/MyTestStruct.cpp)
    file(GLOB_RECURSE headers ${header_path}/*.h)
    file(GLOB_RECURSE scripts ${script_path}/*.${ext})
    include_directories(${header_path})
    include_directories(${TEST_HOME}/Include)
    enable_testing()
	message(STATUS ">>>> SOURCES: ${sources} <<<<")
    add_executable(${testName} ${sources} "${TEST_HOME}/Source/Main.cpp")
    target_link_libraries(${testName} ${core_libs} ${GTEST_BOTH_LIBRARIES} ${Boost_LIBRARIES} gtest_main)
    include(GoogleTest)
    gtest_discover_tests(${testName})
    source_group("Scripts" FILES ${scripts} )
    target_compile_definitions(${testName} PRIVATE KAI_SCRIPT_ROOT=${script_path} KAI_LANG_NAME=${lang} KAI_LANG_EXT=.${ext} gtest gtest_main)
	gtest_add_tests(
         TARGET ${testName}
         SOURCES ${sources}
     )
endforeach()


