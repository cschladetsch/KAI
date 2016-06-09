cmake_minimum_required(VERSION 3.4)

project(Languages)

set(lang_root_dir ${KAI_SOURCE_ROOT_DIR}/Library/Language)

set(langs Pi Rho Tau)
foreach(l ${langs})
    def_lang_library(${l})
endforeach()

