//===- AutocheckDiagnostic.h - Diagnostic formatting helper ---------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares a helper class used for consistent formatting of
// diagnostics.
//
//===----------------------------------------------------------------------===//

#ifndef DIAGNOSTICS_AUTOCHECK_DIAGNOSIC_H
#define DIAGNOSTICS_AUTOCHECK_DIAGNOSIC_H

#include "clang/Basic/Diagnostic.h"
#include "llvm/ADT/StringRef.h"

namespace autocheck {

class AutocheckDiagnostic {
public:
  static void Diag(clang::DiagnosticsEngine &DE,
                   const clang::SourceLocation &Loc, llvm::StringRef Message,
                   llvm::StringRef Rule);
};

} // namespace autocheck

#endif