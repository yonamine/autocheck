cmake_minimum_required(VERSION 3.13.4)

project(autocheck 
  VERSION 0.2.0
  DESCRIPTION "AUTOSAR C++ Guidelines compliance checker"
)

set(AUTOCHECK_ROOT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(AUTOCHECK_SOURCE_DIR ${AUTOCHECK_ROOT_SRC_DIR}/src)
set(LLVM_ROOT_DIR ${AUTOCHECK_SOURCE_DIR}/llvm-project)

# Set C++ standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
file(COPY "${LLVM_BINARY_DIR}/lib/clang/16/include" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/lib/autocheck")

# Attempt to find gtest for unit tests.
# llvm_gtest target is not exposed by LLVM when building out-of-tree.
set(AUTOCHECK_GTEST_AVAILABLE 0)
set(UNITTEST_DIR "${LLVM_ROOT_DIR}/third-party/unittest")
if (EXISTS ${UNITTEST_DIR}/googletest/include/gtest/gtest.h)
  if (NOT TARGET llvm_gtest)
    add_subdirectory(${UNITTEST_DIR} third-party/unittest)
  endif()
  set(AUTOCHECK_GTEST_AVAILABLE 1)
else()
  message(WARNING "Skipping unittests since LLVM install does not include \
    gtest headers and libraries")
  set(AUTOCHECK_GTEST_AVAILABLE 0)
endif()

# Set up include paths.
include_directories(
  ${LLVM_INCLUDE_DIRS}
  ${CLANG_INCLUDE_DIRS}
  ${AUTOCHECK_SOURCE_DIR}/include
  ${CMAKE_BINARY_DIR}/include
)

# Add autocheck targets.
add_subdirectory(src)
if (AUTOCHECK_GTEST_AVAILABLE)
  add_subdirectory(unittests)
endif()
add_subdirectory(test)

# Install autocheck.
include(GNUInstallDirs)
install(TARGETS autocheck RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/autocheck DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Set up CPack.
include(cmake/Pack.cmake)
