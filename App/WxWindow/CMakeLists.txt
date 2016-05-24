cmake_minimum_required(VERSION 3.4)

set(PROJECT_NAME WxWindow)
project(${PROJECT_NAME})


find_package(wxWidgets REQUIRED net gl core base)
include(${wxWidgets_USE_FILE})

set(SOURCES ${KAI_ROOT_SOURCE_DIR}/App/WxWindow/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCES})

include_directories(/usr/local/lib)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# and for each of your dependent executable/library targets:
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
link_directories($ENV{HOME}/lib /usr/local/lib)
link_libraries(Executor Core PiLang RhoLang)
