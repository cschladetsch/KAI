cmake_minimum_required(VERSION 3.5)

project(Library)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

set(SOURCE_FILES
    Source/BuiltinTypes/Array.cpp
    Source/BuiltinTypes/CalcHashValues.cpp
    Source/BuiltinTypes/List.cpp
    Source/BuiltinTypes/Map.cpp
    Source/BuiltinTypes/Set.cpp
    Source/BuiltinTypes/Stack.cpp
    Source/BuiltinTypes/String.cpp
    Source/BasePointerBase.cpp
    Source/BinaryPacket.cpp
    Source/BuiltinTypes.cpp
    Source/Class.cpp
    Source/Debug.cpp
    Source/Documentation.cpp
    Source/DotGraph.cpp
    Source/Exception.cpp
    Source/Label.cpp
    Source/Object.cpp
    Source/Pathname.cpp
    Source/Precompiled.h
    Source/Property.cpp
    Source/Registry.cpp
    Source/StorageBase.cpp
    Source/StringStream.cpp
    Source/Utility.cpp
        )

add_definitions(-DKAI_USE_NAMESPACES -DKAI_NAMESPACE_NAME=kai)

include_directories(/Users/christian/Include /Users/christian/dev/KAI/Include /usr/local/Cellar/boost/1.60.0_2/include)

add_library(Library ${SOURCE_FILES})
