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
#include "clang/Basic/SourceManager.h"
#include <sstream>
#include <utility>

namespace autocheck {

// Storage class for diagnostics information.
struct DiagnosticInfo {
  const char *Message;
  const char *Rule;
  clang::DiagnosticIDs::Level Level;
};

// List of warning messages and rule ids indexed by AutocheckWarnings enum
// value.
const DiagnosticInfo DiagnosticMessages[]{
#define DIAG(ENUM, FLAG, MESSAGE, RULE)                                 \
  {MESSAGE, RULE, clang::DiagnosticIDs::Level::Warning},
#define NOTE(ENUM, MESSAGE) {MESSAGE, "", clang::DiagnosticIDs::Level::Note},
#include "Diagnostics/AutocheckWarnings.def"
};

WarningRepeatChecker &WarningRepeatChecker::getWarningRepeatChecker() {
  static WarningRepeatChecker warningRepeatChecker;
  return warningRepeatChecker;
}

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

bool appropriateHeaderLocation(clang::DiagnosticsEngine &DE,
                               const clang::SourceLocation &Loc) {
  return !Loc.isInvalid() && !((!AutocheckContext::Get().CheckSystemHeaders &&
                                DE.getSourceManager().isInSystemHeader(Loc)) ||
                               (AutocheckContext::Get().DontCheckHeaders &&
                                !DE.getSourceManager().isInMainFile(Loc)));
}

bool appropriateLineLocation(clang::DiagnosticsEngine &DE,
                             const clang::SourceLocation &Loc) {
  if (AutocheckContext::Get().CheckBetweenLines.empty()) {
    return true;
  }

  static const uint32_t FromLine = static_cast<uint32_t>(
      std::stoi(AutocheckContext::Get().CheckBetweenLines[0]));

  static const uint32_t ToLine = static_cast<uint32_t>(
      std::stoi(AutocheckContext::Get().CheckBetweenLines[1]));

  return DE.getSourceManager().getSpellingLineNumber(Loc) >= FromLine &&
         DE.getSourceManager().getSpellingLineNumber(Loc) <= ToLine &&
         DE.getSourceManager().isInMainFile(Loc);
}

bool appropriateLocation(clang::DiagnosticsEngine &DE,
                         const clang::SourceLocation &Loc) {
  return !Loc.isInvalid() && (appropriateHeaderLocation(DE, Loc) &&
                              appropriateLineLocation(DE, Loc));
}

bool shouldIgnoreMacroExpansions(clang::DiagnosticsEngine &DE,
                                 const clang::SourceLocation &SL) {
  return AutocheckContext::Get().DontCheckMacroExpansions &&
         DE.getSourceManager().isMacroBodyExpansion(SL);
}

WarningCounter &getWarningCounter() {
  static WarningCounter WarningCount(AutocheckContext::Get().WarningLimit);
  return WarningCount;
}

AutocheckDiagnosticBuilder::AutocheckDiagnosticBuilder(
    clang::DiagnosticBuilder &DB, clang::DiagnosticsEngine &DE,
    const clang::SourceLocation &Loc, AutocheckWarnings Warning,
    clang::DiagnosticIDs::Level Level)
    : DiagnosticBuilder(DB), DE(DE), Warning(Warning), ReportWarning(true) {
  WarningCounter &WarningCount = getWarningCounter();
  WarningCount.resetLimitPair();
  if (!appropriateLocation(DE, Loc)) {
    ReportWarning = false;
    return;
  }
  if (shouldIgnoreMacroExpansions(DE, Loc)) {
    ReportWarning = false;
    return;
  }
  WarningCount.setLimitPairAndIncrement(Warning);
  if (WarningCount.limitExceeded()) {
    ReportWarning = false;
    return;
  }
  WarningRepeatChecker &warningRepeatChecker =
      WarningRepeatChecker::getWarningRepeatChecker();
  if (warningRepeatChecker.shouldControl(Warning)) {
    if (warningRepeatChecker.updateLineNumber(DE, Loc, Warning)
            .isAlreadyEmitted()) {
      ReportWarning = false;
      return;
    }
  }

  // Save the type of current warning.
  if (Level == clang::DiagnosticIDs::Level::Warning)
    WarningCount.Warning = Warning;
}

AutocheckDiagnosticBuilder::~AutocheckDiagnosticBuilder() {
  if (!ReportWarning) {
    DE.setLastDiagnosticIgnored(true);
    DiagnosticBuilder::Clear();
    DE.Clear();
  }
}

AutocheckWarnings AutocheckDiagnostic::getLatestWarning() {
  return getWarningCounter().Warning;
}

AutocheckDiagnosticBuilder
AutocheckDiagnostic::Diag(clang::DiagnosticsEngine &DE,
                          const clang::SourceLocation &Loc,
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
  return AutocheckDiagnosticBuilder(DB, DE, Loc, Warning, DiagInfo.Level);
}

void AutocheckDiagnostic::addArgsToDiagBuilder(AutocheckDiagnosticBuilder &DB) {
}

} // namespace autocheck
