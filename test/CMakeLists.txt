find_package(Python3)
if(Python3_FOUND)
  message(STATUS "Found python at ${Python3_EXECUTABLE}")
endif()

set(LLVM_LIT_OUTPUT_DIR ${CMAKE_BINARY_DIR}/llvm/bin)

configure_lit_site_cfg(
  ${CMAKE_CURRENT_SOURCE_DIR}/lit.site.cfg.py.in
  ${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg.py
  MAIN_CONFIG
  ${CMAKE_CURRENT_SOURCE_DIR}/lit.cfg.py
  )

set(AUTOCHECK_TEST_DEPENDS
    autocheck
    FileCheck 
    count
    not
)

add_lit_testsuite(check-autocheck "Running autocheck tests"
    ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${AUTOCHECK_TEST_DEPENDS}
)
