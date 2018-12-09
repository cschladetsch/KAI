# setup boost
set(BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE 1)
add_compile_definitions(BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
#set(Boost_DEBUG 1)
set(BOOST_ROOT "c:/Boost/boost_1_66_0")
# for some unknoiwn reason, the following fails, even after setting BOOST_ROOT before calling it. Giving up for now and just hard-coding boost paths :/
# find_package(BOOST COMPONENTS program_options chrono filesystem system)
set(Boost_LIBRARYDIR "c:/Boost/lib")
set(Boost_INCLUDE_DIRS ${BOOST_ROOT})
message(STATUS "Boost IncludeDir: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost Libs: ${Boost_LIBRARYDIR}")

if(!Boost_FOUND)
    message(FATAL_ERROR "We really need boost")
endif()

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARYDIR})


