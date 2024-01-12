//===--- AutocheckTool.h - Main interface to run autocheck ----------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares AutocheckTool which performs AUTOSAR C++ Guidelines
// compilance checks on a file.
//
//===----------------------------------------------------------------------===//

#ifndef AUTOCHECK_H
#define AUTOCHECK_H

#include "clang/Tooling/ArgumentsAdjusters.h"
#include "clang/Tooling/Tooling.h"
#include <string>

class DiagnosticConsumer;

namespace autocheck {

class AutocheckContext;

clang::tooling::ArgumentsAdjuster getResourceDirAdjuster(const char *ExecPath);

/// Utility to check AUTOSAR C++ Guidelines compilance on a file.
class AutocheckTool {
public:
  AutocheckTool(const AutocheckContext &Context,
                clang::tooling::CompilationDatabase &CD,
                const std::string &File);

  /// Append a command line arguments adjuster to the adjuster chain.
  void appendArgumentsAdjuster(clang::tooling::ArgumentsAdjuster Adjuster);

  /// Set a DiagnosticConsumer to handle detected diagnostics.
  void setDiagnosticConsumer(clang::DiagnosticConsumer *DiagConsumer);

  void mapVirtualFile(llvm::StringRef FilePath, llvm::StringRef Content) {
    Tool.mapVirtualFile(FilePath, Content);
  }

  /// Perform check for AUTOSAR C++ Guidelines compliance.
  int run();

private:
  const AutocheckContext &Context;
  clang::tooling::ClangTool Tool;
};

} // namespace autocheck

#endif
