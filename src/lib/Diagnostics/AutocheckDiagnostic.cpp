//===--- AutocheckDiagnostic.cpp - Diagnostic formatting helper -----------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements classes used for formatting and controlling the emission
// of diagnostics.
//
//===----------------------------------------------------------------------===//

#include "Diagnostics/AutocheckDiagnostic.h"

#include "AutocheckContext.h"
#include <sstream>
#include <utility>

namespace autocheck {

// List of warning messages and rule ids indexed by AutocheckWarnings enum
// value.
const std::pair<const char *, const char *> DiagnosticMessages[]{
#define DIAG(ENUM, FLAG, MESSAGE, RULE) {MESSAGE, RULE},
#include "Diagnostics/AutocheckWarnings.def"
};

void WarningCounter::setLimitPairAndIncrement(AutocheckWarnings Warning) {
  // Check if MaxWarning is set to unlimited
  if (MaxWarning == 0) {
    Limits.LimitExceeded = false;
    Limits.LimitReached = false;
    return;
  }
  auto It = Counter.find(Warning);
  if (It == Counter.end()) {
    Counter[Warning] = 1;
    Limits.LimitExceeded = (1 > MaxWarning);
    Limits.LimitReached = (1 == MaxWarning);
    return;
  }

  ++(It->second);
  Limits.LimitExceeded = (It->second > MaxWarning);
  Limits.LimitReached = (It->second == MaxWarning);
  return;
}

void WarningCounter::resetLimitPair() {
  Limits.LimitExceeded = false;
  Limits.LimitReached = false;
}

bool WarningCounter::limitReached() const { return Limits.LimitReached; }
bool WarningCounter::limitExceeded() const { return Limits.LimitExceeded; }

WarningCounter &getWarningCounter() {
  static WarningCounter WarningCount(AutocheckContext::Get().WarningLimit);
  return WarningCount;
}

AutocheckDiagnosticBuilder::AutocheckDiagnosticBuilder(
    clang::DiagnosticBuilder &DB, clang::DiagnosticsEngine &DE,
    const clang::SourceLocation &SL, AutocheckWarnings Warning)
    : DiagnosticBuilder(DB), Loc(Loc), DE(DE), Warning(Warning),
      ReportWarning(true) {
  WarningCounter &WarningCount = getWarningCounter();
  WarningCount.resetLimitPair();
  WarningCount.setLimitPairAndIncrement(Warning);
  if (WarningCount.limitExceeded()) {
    ReportWarning = false;
    return;
  }
}

AutocheckDiagnosticBuilder::~AutocheckDiagnosticBuilder() {
  if (!ReportWarning) {
    DE.setLastDiagnosticIgnored(true);
    DiagnosticBuilder::Clear();
    DE.Clear();
  }
}

AutocheckDiagnosticBuilder
AutocheckDiagnostic::Diag(clang::DiagnosticsEngine &DE,
                          const clang::SourceLocation &Loc,
                          AutocheckWarnings Warning) {
  int WarningID = static_cast<int>(Warning);
  std::stringstream WarningMessage;
  WarningMessage << DiagnosticMessages[WarningID].first << " ["
                 << DiagnosticMessages[WarningID].second << "]";
  unsigned ID = DE.getDiagnosticIDs()->getCustomDiagID(
      clang::DiagnosticIDs::Level::Warning, WarningMessage.str());

  clang::DiagnosticBuilder DB = DE.Report(Loc, ID);
  return AutocheckDiagnosticBuilder(DB, DE, Loc, Warning);
}

void AutocheckDiagnostic::addArgsToDiagBuilder(AutocheckDiagnosticBuilder &DB) {
}

} // namespace autocheck
