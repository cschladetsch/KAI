cmake_minimum_required(VERSION 3.5)
project(Executor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -fpermissive -Wno-deprecated")

set(SOURCE_FILES
    Source/Translator/Pi/PiAstNode.cpp
    Source/Translator/Pi/PiLang.cpp
    Source/Translator/Pi/PiLexer.cpp
    Source/Translator/Pi/PiParser.cpp
    Source/Translator/Pi/PiToken.cpp
    Source/Translator/Pi/PiTranslator.cpp
    Source/Translator/Rho/RhoAstNode.cpp
    Source/Translator/Rho/RhoLang.cpp
    Source/Translator/Rho/RhoLexer.cpp
    Source/Translator/Rho/RhoParser.cpp
    Source/Translator/Rho/RhoToken.cpp
    Source/Translator/Rho/RhoTranslator.cpp
    Source/Translator/LexerBase.cpp
    Source/Translator/Process.cpp
    Source/Translator/TokenBase.cpp
    Source/Translator/TranslatorCommon.cpp
    Source/Bin.cpp
    Source/Compiler.cpp
    Source/Console.cpp
    Source/ConsoleColor.cpp
    Source/Continuation.cpp
    Source/Executor.cpp
    Source/Operation.cpp
    Source/Precompiled.h
    Source/SignedContinuation.cpp
    Source/Tree.cpp)

add_definitions(-DKAI_USE_NAMESPACES -DKAI_NAMESPACE_NAME=kai)

include_directories($ENV{HOME}/Include $ENV{HOME}/dev/KAI/Include /usr/local/Cellar/boost/1.60.0_2/include)

set(dir $ENV{HOME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${dir}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${dir}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")

add_library(Executor ${SOURCE_FILES})
