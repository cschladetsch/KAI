cmake_minimum_required(VERSION 3.28...3.30)

# Performance benchmark tests
project(PerformanceTests)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Bin/Test")

# Performance test sources
set(PERF_SOURCES
    SimplifiedPerformanceTests.cpp
)

# Create the performance test executable
add_executable(PerformanceTests ${PERF_SOURCES})

# Set the runtime output directory for the test executable
set_target_properties(PerformanceTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Bin/Test"
)

# Link libraries - TauLang is only available when networking/Tau is enabled.
set(perf_libs
    RhoLang
    PiLang
    Executor
    Core
    ${GTEST_BOTH_LIBRARIES}
    ${Boost_LIBRARIES}
)

if(TARGET TauLang)
    list(PREPEND perf_libs TauLang)
endif()

target_link_libraries(PerformanceTests ${perf_libs})

# Add include directories
target_include_directories(PerformanceTests PRIVATE
    ${KAI_HOME}/Include
    ${TEST_HOME}/Include
    ${GTEST_INCLUDE_DIRS}
)

# Set compile flags for optimal performance testing
# NOTE: COMPILE_FLAGS (unlike COMPILE_OPTIONS) does not evaluate generator
# expressions - a $<IF:...> here would be written into the build file as
# literal, unresolved text. Use a plain configure-time if/else instead.
if(MSVC)
    set_target_properties(PerformanceTests PROPERTIES COMPILE_FLAGS "/O2")
else()
    set_target_properties(PerformanceTests PROPERTIES COMPILE_FLAGS "-g -O2 -Wno-deprecated")
endif()

# Add the test to CTest
add_test(NAME PerformanceTests COMMAND PerformanceTests)

# Set test properties for longer timeout (performance tests can take time)
set_tests_properties(PerformanceTests PROPERTIES TIMEOUT 300)
