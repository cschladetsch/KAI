cmake_minimum_required(VERSION 3.5)
project(Testibrary)

include($ENV{HOME}/dev/KAI/CMakeLists.common.txt)

set(SOURCE_FILES
    Source/Main.cpp
    Source/TestBinaryStream.cpp
    Source/TestClassScripting.cpp
    Source/TestCompilerBasic.cpp
    Source/TestDotGraph.cpp
    Source/TestFunction.cpp
    Source/TestFunctionScripting.cpp
    Source/TestPi.cpp
    Source/TestPiExecutor.cpp
    Source/TestPiParser.cpp
    Source/TestPiString.cpp
    Source/TestPiTokeniser.cpp
    Source/TestRhoScripting.cpp)

link_directories($ENV{HOME}/lib /usr/local/lib)

link_libraries(Executor Library gtest)
find_package (Threads)
add_executable(TestLibrary ${SOURCE_FILES})

target_link_libraries (TestLibrary ${CMAKE_THREAD_LIBS_INIT})