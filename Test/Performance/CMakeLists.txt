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
# NOTE: COMPILE_FLAGS (unlike COMPILE_OPTIONS/target_compile_options) does not
# evaluate generator expressions - a $<IF:...> or $<CONFIG:...> there would be
# written into the build file as literal, unresolved text.
#
# /O2 must NOT be forced in Debug on MSVC: MSVC's Debug configuration adds
# /RTC1 (runtime checks) by default, and cl.exe rejects /O2 + /RTC1 together
# with "error D8016: '/O2' and '/RTC1' command-line options are
# incompatible", which fails the whole build (not just this target's
# optimization). target_compile_options() does evaluate generator
# expressions, so use it (instead of the COMPILE_FLAGS property above) to
# apply /O2 only outside Debug/RelWithDebInfo-with-RTC configs.
if(MSVC)
    target_compile_options(PerformanceTests PRIVATE
        $<$<NOT:$<CONFIG:Debug>>:/O2>
    )
else()
    target_compile_options(PerformanceTests PRIVATE -g -O2 -Wno-deprecated)
endif()

# Add the test to CTest
add_test(NAME PerformanceTests COMMAND PerformanceTests)

# Set test properties for longer timeout (performance tests can take time)
set_tests_properties(PerformanceTests PROPERTIES TIMEOUT 300)
