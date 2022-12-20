cmake_minimum_required(VERSION 3.13.4)

project(autocheck)

set(AUTOCHECK_ROOT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if (NOT CLANG_DIR)
  message(FATAL_ERROR "CLANG_DIR must be specified in order to find AddClang")
endif()

add_subdirectory(src/)
