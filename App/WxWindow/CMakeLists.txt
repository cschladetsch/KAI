cmake_minimum_required(VERSION 3.4)

set(PROJECT_NAME WxWindow)
project(${PROJECT_NAME})

set(SOURCES ${KAI_ROOT_SOURCE_DIR}/App/WxWindow/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_wxWidgets(${PROJECT_NAME})


# and for each of your dependent executable/library targets:
link_libraries(Executor Core PiLang RhoLang)

