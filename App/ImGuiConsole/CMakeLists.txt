cmake_minimum_required(VERSION 3.4)

set(name ImGuiConsole)
project(${name})


find_package(OpenGL REQUIRED)
find_package(Cocoa REQUIRED)
find_package(IOKit REQUIRED)
find_package(CoreVideo REQUIRED)

message(STATUS "OPENGL_INCLUDE_DIR: ${OPENGL_INCLUDE_DIR}")

include_directories(${OPENGL_INCLUDE_DIR})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework OpenGl")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework IOKit")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework CoreVideo")

# TODO: use KAI_PLATFORM_OSX
add_definitions(-DKAI_PLATFORM_OSX)
add_definitions(-D__APPLE__)

include_directories(/opt/local/include ${KAI_HOME}/Include/Imgui)
link_directories(/opt/local/lib)

set(SOURCE_ROOT ${KAI_HOME}/App/${name}/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

link_libraries(Executor Core PiLang RhoLang Imgui)

add_executable(${name} ${SOURCE_FILES})

# OpenGl
target_link_libraries(${name} glfw)

cotire(${name})
