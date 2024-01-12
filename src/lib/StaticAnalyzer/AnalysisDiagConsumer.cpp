//===--- AnalysisDiagConsumer.cpp - Diagnostic consumer for SA checks -----===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file implements AutocheckAnalysisDiagConsumer.
//
//===----------------------------------------------------------------------===//

#include "StaticAnalyzer/AnalysisDiagConsumer.h"
#include "Diagnostics/AutocheckDiagnostic.h"

namespace autocheck {

AutocheckAnalysisDiagConsumer::AutocheckAnalysisDiagConsumer(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

llvm::StringRef AutocheckAnalysisDiagConsumer::getName() const {
  return "Autocheck Diagnostic Consumer";
}

bool AutocheckAnalysisDiagConsumer::supportsLogicalOpControlFlow() const {
  return true;
}
bool AutocheckAnalysisDiagConsumer::supportsCrossFileDiagnostics() const {
  return true;
}

void AutocheckAnalysisDiagConsumer::FlushDiagnosticsImpl(
    std::vector<const clang::ento::PathDiagnostic *> &Diags,
    FilesMade *FilesMade) {
  for (auto &Diag : Diags) {
    if (Diag->getCheckerName() == "autosar.DivZeroChecker")
      AutocheckDiagnostic::reportWarning(DE, Diag->getLocation().asLocation(),
                                         AutocheckWarnings::divByZero);
    else if (Diag->getCheckerName() == "autosar.UnreachableCodeChecker") {
      AutocheckDiagnostic::reportWarning(DE, Diag->getLocation().asLocation(),
                                         AutocheckWarnings::unreachableCode);
    } else {
      // For all other checkers display their description through autochecks
      // report system so we can track diagnostics properly.
      unsigned WarnID =
          DE.getCustomDiagID(clang::DiagnosticsEngine::Warning, "%0");
      DE.Report(Diag->getLocation().asLocation(), WarnID)
          << Diag->getShortDescription();
    }
  }
}

} // namespace autocheck
