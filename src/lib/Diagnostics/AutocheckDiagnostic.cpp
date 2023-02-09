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

#include <sstream>
#include <utility>

namespace autocheck {

// List of warning messages and rule ids indexed by AutocheckWarnings enum
// value.
const std::pair<const char *, const char *> DiagnosticMessages[]{
#define DIAG(ENUM, FLAG, MESSAGE, RULE) {MESSAGE, RULE},
#include "Diagnostics/AutocheckWarnings.def"
};

clang::DiagnosticBuilder
AutocheckDiagnostic::Diag(clang::DiagnosticsEngine &DE,
                          const clang::SourceLocation &Loc,
                          AutocheckWarnings Warning) {
  int WarningID = static_cast<int>(Warning);
  std::stringstream WarningMessage;
  WarningMessage << DiagnosticMessages[WarningID].first << " ["
                 << DiagnosticMessages[WarningID].second << "]";
  unsigned ID = DE.getDiagnosticIDs()->getCustomDiagID(
      clang::DiagnosticIDs::Level::Warning, WarningMessage.str());

  return DE.Report(Loc, ID);
}

void AutocheckDiagnostic::addArgsToDiagBuilder(clang::DiagnosticBuilder &DB) {}

} // namespace autocheck
