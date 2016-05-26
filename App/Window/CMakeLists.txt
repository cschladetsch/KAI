cmake_minimum_required(VERSION 3.4)

set(PROJECT_NAME Window)
project(${PROJECT_NAME})

set(root ${KAI_ROOT_SOURCE_DIR}/App/${PROJECT_NAME})
include_directories(${include_directories} ${root})

set(SOURCES ${root}/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCES})
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_wxWidgets(${PROJECT_NAME})

# and for each of your dependent executable/library targets:
link_libraries(Executor Core PiLang RhoLang)


