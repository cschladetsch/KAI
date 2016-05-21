cmake_minimum_required(VERSION 3.4)

project(Language)

#set(langs Pi Rho Tau)
#
#foreach(lang ${langs})
#    set(SOURCE_ROOT ${KAI_ROOT_SOURCE_DIR}/Library/Language/${lang})
#    file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
#endforeach()
#
#add_library(Language ${SOURCE_FILES})

set(SOURCE_ROOT ${KAI_ROOT_SOURCE_DIR}/Library/Language/Pi/Source)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
add_library(Language ${SOURCE_FILES})


#$