include_directories(${TEST_HOME}/Include})

if (KAI_BUILD_CORE_TEST_ALL)
    add_subdirectory(Source)
else()
    message(WARNING "Not building KAI Core unit tests")
endif()

if (KAI_BUILD_TEST_LANG)
    add_subdirectory(Language)
else()
    message(WARNING "Not building Language unit tests")
endif()

if (KAI_BUILD_TEST_NETWORK)
    add_subdirectory(Network)
else()
    message(WARNING "Not building Network unit tests")
endif()


