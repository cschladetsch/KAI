#set(Boost_DEBUG 1)

message(STATUS "Looking for boost")

if (WIN32)
	set(Boost_NO_SYSTEM_PATHS on CACHE BOOL "Do not search system for Boost")
	set(BOOST_ROOT "c:/boost/boost_1_75_0" CACHE PATH "Boost library path")
	set(Boost_INCLUDE_DIR ${CMAKE_INCLUDE_PATH} "c:/boost/boost_1_75_0")
	set(Boost_LIBRARY_DIR ${CMAKE_LIBRARY_PATH} "c:/boost/boost_1_75_0/stage/lib")
endif()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_DEBUG_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.67.0 REQUIRED COMPONENTS 
    program_options chrono filesystem system date_time)

message(STATUS "====> Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "====> Boost_LIBRARY_DIR_DEBUG: ${Boost_LIBRARY_DIR_DEBUG}")
message(STATUS "====> Boost_LIBRARIES: ${Boost_LIBRARIES}")

include_directories(${Boost_INCLUDE_DIRS})

