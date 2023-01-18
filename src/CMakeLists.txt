include_directories(
    ${LLVM_INCLUDE_DIRS}
    ${CLANG_INCLUDE_DIRS}
    ${AUTOCHECK_ROOT_SRC_DIR}/src/include
)

set(LLVM_LINK_COMPONENTS
    ${LLVM_TARGETS_TO_BUILD}
    Option
    Support
)

add_clang_executable(autocheck 
    "lib/Autocheck.cpp"
    "lib/AutocheckAction.cpp"
    "lib/AutocheckContext.cpp"
    "lib/Diagnostics/AutocheckDiagnostic.cpp"
    "lib/Diagnostics/AutocheckDiagnosticConsumer.cpp"
)
clang_target_link_libraries(autocheck
    PRIVATE
    clangBasic
    clangFrontend
    clangStaticAnalyzerFrontend
    clangTooling
)
set_target_properties(autocheck
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)