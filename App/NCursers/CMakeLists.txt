cmake_minimum_required(VERSION 3.4)

set(proj_name NCursers)
project(${proj_name})

set(SOURCE_ROOT ${KAI_HOME}/App/${proj_name}/Source/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})

link_directories(${HOME}/lib /usr/local/lib)
link_libraries(Executor Core PiLang RhoLang)

#$message(WARNING -- ${SOURCE_FILES} == ${KAI_HOME})

add_executable(${proj_name} ${SOURCE_FILES} )

target_link_libraries(${proj_name} /usr/lib/x86_64-linux-gnu/libcurses.so)
