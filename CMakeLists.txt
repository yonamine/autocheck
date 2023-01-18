cmake_minimum_required(VERSION 3.13.4)

project(autocheck)

set(AUTOCHECK_ROOT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Find clang.
if (NOT CLANG_DIR)
  message(FATAL_ERROR "CLANG_DIR must be specified in order to find AddClang")
endif()

find_package(Clang REQUIRED CONFIG PATHS ${CLANG_DIR} NO_DEFAULT_PATH)
message(STATUS "Using ClangConfig.cmake in: ${CLANG_DIR}")

# Configure LLVM/Clang.
list(APPEND CMAKE_MODULE_PATH ${LLVM_CMAKE_DIR} ${CLANG_CMAKE_DIR})
include(AddLLVM)
include(AddClang)

# Copy clang built-in headers.
file(COPY "${LLVM_BINARY_DIR}/lib/clang" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/lib")

# Add autocheck targets.
add_subdirectory(src)
add_subdirectory(test)
