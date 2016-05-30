cmake_minimum_required(VERSION 3.4)
project(Window)

set(Root ${KAI_HOME}/App/Window/Source)

file(GLOB_RECURSE SOURCE_FILES ${Root}/*.cpp)
file(GLOB_RECURSE WINDOW_FILES ${Root}/*.cxx)

#include_directories(/usr/local/include/)
#message(WARNING -- ${SOURCE_FILES} -- ${WINDOW_FILES} --)

add_executable(Window ${SOURCE_FILES} ${WINDOW_FILES} Fluid/Window.h)

link_directories(/usr/lib/x86_64-linux-gnu)

set(x11libs Xcursor Xfixes Xext Xft Xinerama fontconfig X11 pthread)

target_link_libraries(Window fltk fltk_forms fltk_gl fltk_images z dl ${x11libs})

