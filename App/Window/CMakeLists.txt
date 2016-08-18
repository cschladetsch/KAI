cmake_minimum_required(VERSION 3.4)

project(Window)

set(Root ${KAI_HOME}/App/Window/Source)

file(GLOB_RECURSE SOURCE_FILES ${Root}/*.cpp)
file(GLOB_RECURSE WINDOW_FILES ${Root}/*.cxx)

set(x11libs Xfixes Xext Xft fontconfig X11 pthread)

link_libraries(Executor Core PiLang RhoLang)

add_executable(Window ${SOURCE_FILES} ${WINDOW_FILES})

target_link_libraries(Window fltk fltk_forms fltk_gl fltk_images z dl ${x11libs})
