set(LLVM_LINK_COMPONENTS
    ${LLVM_TARGETS_TO_BUILD}
    Option
    Support
)

configure_file(include/Version.h.in ${CMAKE_BINARY_DIR}/include/Version.h)

add_clang_executable(autocheck 
    "lib/Autocheck.cpp"
    "lib/AutocheckAction.cpp"
    "lib/AutocheckContext.cpp"
    "lib/Diagnostics/AutocheckDiagnostic.cpp"
    "lib/Diagnostics/AutocheckDiagnosticConsumer.cpp"
    "lib/Diagnostics/DiagnosticHelper.cpp"
    "lib/Export/DiagExporter.cpp"
    "lib/Export/DiagExporterJson.cpp"
    "lib/Lex/AutocheckLex.cpp"
    "lib/Lex/AutocheckPPCallbacks.cpp"
    "lib/Lex/CharHelper.cpp"
    "lib/Lex/LiteralHelper.cpp"
    "lib/AST/ASTHelper.cpp"
    "lib/AST/LexicalRulesVisitor.cpp"
    "lib/AST/StatementsVisitor.cpp"
    "lib/AST/ClassesVisitor.cpp"
    "lib/AST/DeclarationsVisitor.cpp"
    "lib/AST/ExpressionsVisitor.cpp"
    "lib/AST/ConversionsVisitor.cpp"
    "lib/AST/TypesVisitor.cpp"
    "lib/AST/LoopsVisitor.cpp"
    "lib/AST/HeadersVisitor.cpp"
    "lib/AST/Matchers.cpp"
    "lib/AST/TemplatesVisitor.cpp"
    "lib/AST/SideEffectChecker.cpp"
    "lib/StaticAnalyzer/DivZeroChecker.cpp"
    "lib/StaticAnalyzer/RecursionChecker.cpp"
    "lib/StaticAnalyzer/UnreachableCodeChecker.cpp"
)
clang_target_link_libraries(autocheck
    PRIVATE
    clangAST
    clangBasic
    clangFrontend
    clangLex
    clangStaticAnalyzerFrontend
    clangTooling
    clangToolingSyntax
)
set_target_properties(autocheck
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
