cmake_minimum_required(VERSION 3.4)

project(Languages)

#set(langs Pi Rho Tau)
#
#foreach(lang ${langs})
#    set(SOURCE_ROOT ${KAI_ROOT_SOURCE_DIR}/Library/Language/${lang})
#    file(GLOB_RECURSE SOURCE_FILES ${SOURCE_ROOT})
#endforeach()
#
#add_library(Language ${SOURCE_FILES})

set(lang_root_dir ${KAI_SOURCE_ROOT_DIR}/Library/Language)
include_directories(${lang_root_dir}/Include)

message(${lang_root_dir})

def_lang_library(Pi)
def_lang_library(Rho)

#$