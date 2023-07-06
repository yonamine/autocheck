set(LLVM_LINK_COMPONENTS support)

add_clang_executable(autocheck
    "Driver.cpp"
)
clang_target_link_libraries(autocheck PRIVATE libautocheck)
set_target_properties(autocheck
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
