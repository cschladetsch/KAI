cmake_minimum_required(VERSION 3.4)

project(Languages)

set(lang_root_dir ${KAI_SOURCE_ROOT_DIR}/Library/Language)
include_directories(${lang_root_dir}/Window)

def_lang_library(Pi)
def_lang_library(Rho)
def_lang_library(Tau)

#$