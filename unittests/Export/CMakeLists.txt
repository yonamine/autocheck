add_autocheck_unittest(ExportTests
  "${AUTOCHECK_SOURCE_DIR}/lib/AutocheckContext.cpp"
  "${AUTOCHECK_SOURCE_DIR}/lib/Diagnostics/AutocheckDiagnostic.cpp"
  "${AUTOCHECK_SOURCE_DIR}/lib/Diagnostics/DiagnosticHelper.cpp"
  "${AUTOCHECK_SOURCE_DIR}/lib/Export/DiagExporter.cpp"
  "${AUTOCHECK_SOURCE_DIR}/lib/Export/DiagExporterJson.cpp"
  "DiagExporterJsonTest.cpp"
)
clang_target_link_libraries(ExportTests PRIVATE clangBasic)
