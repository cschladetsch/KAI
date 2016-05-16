cmake_minimum_required(VERSION 3.5)
project(Executor)

include($ENV{HOME}/dev/KAI/CMakeLists.common.txt)

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

add_library(Executor ${SOURCE_FILES})
