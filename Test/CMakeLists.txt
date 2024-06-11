include_directories(${TEST_HOME}/Include})

add_subdirectory(Source)
add_subdirectory(Language)
#add_subdirectory(Common)

if (KAI_BUILD_TEST_NETWORK)
    add_subdirectory(Network)
else()
    message(WARNING "Not building Network unit tests")
endif()


