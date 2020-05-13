set(Boost_DEBUG 1)

message(STATUS "Looking for boost")

#set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/Boost/include")
#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/Boost/lib")

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

