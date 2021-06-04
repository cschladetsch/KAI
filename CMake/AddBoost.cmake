#set(Boost_DEBUG 1)

message(STATUS "Looking for boost")

set(BOOST_ROOT C:/lib/boost/boost_1_75_0)

set(BOOST_INCLUDE_DIR ${BOOST_ROOT})
set(BOOST_LIBRARYDIR ${BOOST_ROOT}/libs)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_DEBUG_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS 
    program_options chrono filesystem system date_time)

message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARY_DIR_DEBUG: ${Boost_LIBRARY_DIR_DEBUG}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")

include_directories(${Boost_INCLUDE_DIRS})

