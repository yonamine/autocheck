cmake_minimum_required(VERSION 3.13.4)

project(autocheck 
  VERSION 0.1.4
  DESCRIPTION "AUTOSAR C++ Guidelines compliance checker"
)

set(AUTOCHECK_ROOT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

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

# Add autocheck targets.
add_subdirectory(src)
add_subdirectory(test)

# Install autocheck.
include(GNUInstallDirs)
install(TARGETS autocheck RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/autocheck DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Set up CPack.
include(cmake/Pack.cmake)
