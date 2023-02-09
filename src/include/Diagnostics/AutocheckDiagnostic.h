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

#include "Diagnostics/AutocheckWarnings.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/SourceLocation.h"

namespace autocheck {

class AutocheckDiagnostic {
public:
  AutocheckDiagnostic() = delete;

  template <typename... ArgsType>
  static void reportWarning(clang::DiagnosticsEngine &DE,
                            const clang::SourceLocation &Loc,
                            AutocheckWarnings Warning, ArgsType &&...Args) {
    clang::DiagnosticBuilder DB = Diag(DE, Loc, Warning);
    addArgsToDiagBuilder(DB, std::forward<ArgsType>(Args)...);
  }

private:
  static clang::DiagnosticBuilder Diag(clang::DiagnosticsEngine &DE,
                                       const clang::SourceLocation &Loc,
                                       AutocheckWarnings Warning);

  // Base case for variadic template.
  static void addArgsToDiagBuilder(clang::DiagnosticBuilder &DB);

  template <typename ArgType, typename... ArgsType>
  static void addArgsToDiagBuilder(clang::DiagnosticBuilder &DB, ArgType &&Arg,
                                   ArgsType &&...Args) {
    DB << std::forward<ArgType>(Arg);
    addArgsToDiagBuilder(DB, std::forward<ArgsType>(Args)...);
  }
};

} // namespace autocheck

#endif
