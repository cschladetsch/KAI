cmake_minimum_required(VERSION 3.5)

project(TestLibrary)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Main.cpp
    Precompiled.h
    TestBase.h
    TestBinaryStream.cpp
    TestClassScripting.cpp
    TestCompilerBasic.cpp
    TestDotGraph.cpp
    TestFunction.cpp
    TestFunctionScripting.cpp
    TestPi.cpp
    TestPiExecutor.cpp
    TestPiParser.cpp
    TestPiString.cpp
    TestPiTokeniser.cpp
    TestRhoScripting.cpp)

link_directories($(HOME)/lib)

target_link_libraries(Library Executable gtest)

include_directories($(HOME)/include $(HOME)/dev/KAI/Include)

add_executable(Library ${SOURCE_FILES})


