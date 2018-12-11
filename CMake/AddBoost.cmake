set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_DEBUG_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
#set(BOOST_ROOT "c:/Boost/boost_1_68_0")
#set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/stage/lib")
find_package(Boost REQUIRED COMPONENTS program_options chrono filesystem system)

message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARY_DIR_DEBUG: ${Boost_LIBRARY_DIR_DEBUG}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")

include_directories(${Boost_INCLUDE_DIRS})

