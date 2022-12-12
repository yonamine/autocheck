cmake_minimum_required(VERSION 3.13.4)

project(autocheck)

set(AUTOCHECK_ROOT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if (NOT LLVM_DIR)
  message(FATAL_ERROR "LLVM_DIR must be specified in order to find LLVMConfig")
endif()

add_subdirectory(src/llvm-project/llvm)
add_subdirectory(src/)
