//===--- AutocheckDiagnostic.cpp - Diagnostic formatting helper -----------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the AutocheckDiagnostic class.
//
//===----------------------------------------------------------------------===//

#include "Diagnostics/AutocheckDiagnostic.h"

namespace autocheck {

void AutocheckDiagnostic::Diag(clang::DiagnosticsEngine &DE,
                               const clang::SourceLocation &Loc,
                               llvm::StringRef Message, llvm::StringRef Rule) {
  unsigned ID = DE.getDiagnosticIDs()->getCustomDiagID(
      clang::DiagnosticIDs::Level::Warning,
      Message.str() + " [" + Rule.str() + "]");
  DE.Report(Loc, ID);
}

} // namespace autocheck
