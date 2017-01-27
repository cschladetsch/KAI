cmake_minimum_required(VERSION 3.4)

project(Languages)

set(lang_root_dir ${KAI_SOURCE_ROOT_DIR}/Library/Language)

# Ubuntu only set(langs Common Pi Rho Tau)
set(langs Common Pi Rho)

foreach(l ${langs})
    def_lang_library(${l})
endforeach()

