//===--- AutocheckDiagnostic.cpp - Diagnostic formatting helper -----------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file implements classes used for formatting and controlling the emission
// of diagnostics.
//
//===----------------------------------------------------------------------===//

#include "Diagnostics/AutocheckDiagnostic.h"
#include "AutocheckContext.h"
#include "clang/Basic/SourceManager.h"
#include <sstream>
#include <utility>

namespace autocheck {

// List of warning messages and rule ids indexed by AutocheckWarnings enum
// value.
const DiagnosticInfo DiagnosticMessages[]{
#define DIAG(ENUM, FLAG, MESSAGE, RULE)                                        \
  {MESSAGE, RULE, clang::DiagnosticIDs::Level::Warning},
#define NOTE(ENUM, MESSAGE) {MESSAGE, "", clang::DiagnosticIDs::Level::Note},
#include "Diagnostics/AutocheckWarnings.def"
};

WarningRepeatChecker::WarningRepeatChecker() {
  PreviousLineNumbers = std::unordered_map<AutocheckWarnings, unsigned>({
      {AutocheckWarnings::nonCppStandardCharUsed, -1},
      {AutocheckWarnings::hashhashOpUsed, -1},
      {AutocheckWarnings::hexConstUpperCase, -1},
      {AutocheckWarnings::similarIdentifiers, -1},
      {AutocheckWarnings::fixedWidthIntegerTypes, -1},
      {AutocheckWarnings::memberDataPrivate, -1},
      {AutocheckWarnings::nonBracedInit, -1},
      {AutocheckWarnings::functionRedeclParams, -1},
      {AutocheckWarnings::broadScopeIdentifier, -1},
      {AutocheckWarnings::constUnusedForImmutableData, -1},
      {AutocheckWarnings::cvQualifiersPlacedLeft, -1},
      {AutocheckWarnings::inParamPassedByValue, -1},
      {AutocheckWarnings::ternaryOpSubExpr, -1},
      {AutocheckWarnings::implicitBitwiseBinopConversion, -1},
      {AutocheckWarnings::incDecOpMixed, -1},
      {AutocheckWarnings::invalidCharExpression, -1},
      {AutocheckWarnings::invalidEnumExpression, -1},
      {AutocheckWarnings::implicitFloatIntegralConversion, -1},
      {AutocheckWarnings::zeroToNullPointer, -1},
      {AutocheckWarnings::arrayDecaysToPointer, -1},
      {AutocheckWarnings::nullptrOnlyNullPtrConst, -1},
      {AutocheckWarnings::implicitSizeReductionConversion, -1},
      {AutocheckWarnings::charStorage, -1},
      {AutocheckWarnings::contiditionNotBool, -1},
      {AutocheckWarnings::cStyleArrayUsed, -1},
      {AutocheckWarnings::longDoubleUsed, -1},
      {AutocheckWarnings::notAndOrOpsBoolOperands, -1},
      {AutocheckWarnings::bitwiseOperandNotUnsigned, -1},
      {AutocheckWarnings::impcastChangesSignedness, -1},
      {AutocheckWarnings::commaOperatorUsed, -1},
      {AutocheckWarnings::cStyleCastUsed, -1},
      {AutocheckWarnings::typedefUsed, -1},
      {AutocheckWarnings::assignmentOpSubExpr, -1},
  });
}

bool WarningRepeatChecker::shouldControl(AutocheckWarnings Warning) {
  return PreviousLineNumbers.count(Warning);
}

WarningRepeatChecker::LineNumbers
WarningRepeatChecker::updateLineNumber(clang::DiagnosticsEngine &DE,
                                       const clang::SourceLocation &Loc,
                                       AutocheckWarnings Warning) {
  clang::SourceManager &SM = DE.getSourceManager();
  LineNumbers DiagLineNumbers;
  DiagLineNumbers.Previous = PreviousLineNumbers[Warning];
  DiagLineNumbers.Current =
      SM.getExpansionLoc(Loc).getRawEncoding() -
      SM.getExpansionColumnNumber(SM.getExpansionLoc(Loc));

  PreviousLineNumbers[Warning] = DiagLineNumbers.Current;

  return DiagLineNumbers;
}

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

bool appropriateHeaderLocation(AutocheckDiagnostic &AD,
                               const clang::SourceLocation &Loc) {
  const clang::SourceManager &SM = AD.GetSourceManager();
  return !Loc.isInvalid() &&
         !((!AD.GetContext().CheckSystemHeaders && SM.isInSystemHeader(Loc)) ||
           (AD.GetContext().DontCheckHeaders && !SM.isInMainFile(Loc)));
}

static bool appropriateLineLocation(AutocheckDiagnostic &AD,
                                    const clang::SourceLocation &Loc) {
  if (AD.GetContext().CheckBetweenLines.empty()) {
    return true;
  }

  static const uint32_t FromLine =
      static_cast<uint32_t>(std::stoi(AD.GetContext().CheckBetweenLines[0]));

  static const uint32_t ToLine =
      static_cast<uint32_t>(std::stoi(AD.GetContext().CheckBetweenLines[1]));

  const clang::SourceManager &SM = AD.GetSourceManager();
  return SM.getSpellingLineNumber(Loc) >= FromLine &&
         SM.getSpellingLineNumber(Loc) <= ToLine && SM.isInMainFile(Loc);
}

bool appropriateLocation(AutocheckDiagnostic &AD,
                         const clang::SourceLocation &Loc) {
  return !Loc.isInvalid() && (appropriateHeaderLocation(AD, Loc) &&
                              appropriateLineLocation(AD, Loc));
}

bool shouldIgnoreMacroExpansions(AutocheckDiagnostic &AD,
                                 const clang::SourceLocation &SL) {
  return AD.GetContext().DontCheckMacroExpansions &&
         AD.GetSourceManager().isMacroBodyExpansion(SL);
}

AutocheckDiagnosticBuilder::AutocheckDiagnosticBuilder(
    clang::DiagnosticBuilder &DB, AutocheckDiagnostic &AD,
    const clang::SourceLocation &Loc, AutocheckWarnings Warning,
    clang::DiagnosticIDs::Level Level)
    : DiagnosticBuilder(DB), DE(AD.GetDiagnostics()), Warning(Warning),
      ReportWarning(true) {
  WarningCounter &WarningCount = AD.Counter;
  WarningCount.resetLimitPair();
  if (!appropriateLocation(AD, Loc)) {
    ReportWarning = false;
    return;
  }
  if (shouldIgnoreMacroExpansions(AD, Loc)) {
    ReportWarning = false;
    return;
  }
  WarningCount.setLimitPairAndIncrement(Warning);
  if (WarningCount.limitExceeded()) {
    ReportWarning = false;
    return;
  }

  WarningRepeatChecker &WarningRepeatChecker = AD.RepeatChecker;
  if (WarningRepeatChecker.shouldControl(Warning)) {
    if (WarningRepeatChecker.updateLineNumber(DE, Loc, Warning)
            .isAlreadyEmitted()) {
      ReportWarning = false;
      return;
    }
  }

  // Save the type of current warning.
  if (Level == clang::DiagnosticIDs::Level::Warning)
    WarningCount.Warning = Warning;

  // Notify observers when a warning is emitted.
  if (ReportWarning && Level == clang::DiagnosticIDs::Level::Warning) {
    std::for_each(AD.Listeners.begin(), AD.Listeners.end(),
                  [&Warning, Loc](DiagListener *L) {
                    L->OnDiagnosticEmitted(Warning, Loc);
                  });
  }
}

AutocheckDiagnosticBuilder::~AutocheckDiagnosticBuilder() {
  if (!ReportWarning) {
    DE.setLastDiagnosticIgnored(true);
    DiagnosticBuilder::Clear();
    DE.Clear();
  }
}

AutocheckDiagnostic::AutocheckDiagnostic(const AutocheckContext &Context,
                                         clang::DiagnosticsEngine &DE)
    : Context(Context), DE(DE), Counter(Context.WarningLimit) {}

AutocheckWarnings AutocheckDiagnostic::getLatestWarning() const {
  return Counter.Warning;
}

AutocheckDiagnosticBuilder
AutocheckDiagnostic::Diag(const clang::SourceLocation &Loc,
                          AutocheckWarnings Warning) {
  int DiagID = static_cast<int>(Warning);
  const DiagnosticInfo &DiagInfo = DiagnosticMessages[DiagID];
  std::stringstream DiagMessage;
  DiagMessage << DiagInfo.Message;
  if (DiagInfo.Level == clang::DiagnosticIDs::Level::Warning)
    DiagMessage << " [" << DiagInfo.Rule << "]";
  unsigned ID =
      DE.getDiagnosticIDs()->getCustomDiagID(DiagInfo.Level, DiagMessage.str());

  clang::DiagnosticBuilder DB = DE.Report(Loc, ID);
  return AutocheckDiagnosticBuilder(DB, *this, Loc, Warning, DiagInfo.Level);
}

void AutocheckDiagnostic::addArgsToDiagBuilder(AutocheckDiagnosticBuilder &DB) {
}

bool AutocheckDiagnostic::AddDiagListener(DiagListener *DL) {
  const auto Result = Listeners.insert(DL);
  return Result.second;
}

bool AutocheckDiagnostic::RemoveDiagListener(DiagListener *DL) {
  return Listeners.erase(DL);
}

const DiagnosticInfo &
AutocheckDiagnostic::GetDiagInfo(AutocheckWarnings Warning) {
  return DiagnosticMessages[static_cast<int>(Warning)];
}

const AutocheckContext &AutocheckDiagnostic::GetContext() const {
  return Context;
}

bool AutocheckDiagnostic::IsEnabled(AutocheckWarnings Warning) const {
  return Context.isEnabled(Warning);
}

clang::DiagnosticsEngine &AutocheckDiagnostic::GetDiagnostics() { return DE; }

clang::SourceManager &AutocheckDiagnostic::GetSourceManager() const {
  return DE.getSourceManager();
}

} // namespace autocheck
