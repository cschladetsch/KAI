cmake_minimum_required(VERSION 3.7)

set(name GuiConsole)
project(${name})

set(frameworks OpenGL Cocoa IOKit CoreVideo)

foreach(framework ${frameworks})
	# include(${CMAKE_MODULE_PATH}/Find${framework}.cmake)
	set(${framework}_FIND_QUIETLY true)
	find_package(${framework})
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework ${framework}")
endforeach()

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
