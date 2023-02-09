//===- AutocheckDiagnostic.h - Diagnostic formatting helper ---------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares classes used for formatting and controlling the emission
// of diagnostics.
//
//===----------------------------------------------------------------------===//

#ifndef DIAGNOSTICS_AUTOCHECK_DIAGNOSIC_H
#define DIAGNOSTICS_AUTOCHECK_DIAGNOSIC_H

#include "Diagnostics/AutocheckWarnings.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/SourceLocation.h"
#include <unordered_map>

namespace autocheck {

struct LimitPair {
  bool LimitExceeded;
  bool LimitReached;
};

class WarningCounter {
public:
  WarningCounter(const unsigned MaxWarning) : MaxWarning(MaxWarning) {}

  /// For a given AutocheckWarning, this method:
  /// - sets the corresponding LimitPair by consulting the number of reported
  ///   warnings from the warning counter map
  /// - increments number of reported warnings within the warning counter map
  void setLimitPairAndIncrement(AutocheckWarnings Warning);
  bool limitReached() const;
  bool limitExceeded() const;
  void resetLimitPair();

private:
  /// MaxWarning has the same value as Context.WarningLimit.
  const unsigned MaxWarning;
  /// Map each rule to the number of already reported warnings for that rule.
  std::unordered_map<AutocheckWarnings, unsigned> Counter;
  /// This is a bool pair that holds current information whether the per-rule
  /// warning limit is reached and/or exceeded for the last reported diagnostic
  /// (which may not have been shown to the user because the limit was reached).
  LimitPair Limits;
};

WarningCounter &getWarningCounter();

/// Custom DiagnosticsBuilder class that checks autocheck diagnostics options to
/// determine whether to report the diagnostic to the user or not.
class AutocheckDiagnosticBuilder : public clang::DiagnosticBuilder {
public:
  AutocheckDiagnosticBuilder(clang::DiagnosticBuilder &DB,
                             clang::DiagnosticsEngine &DE,
                             const clang::SourceLocation &Loc,
                             AutocheckWarnings Warning);

  ~AutocheckDiagnosticBuilder();

  template <typename T>
  friend const AutocheckDiagnosticBuilder &
  operator<<(const AutocheckDiagnosticBuilder &Diag, const T &Value) {
    const DiagnosticBuilder &BaseDiag = Diag;
    BaseDiag << Value;
    return Diag;
  }

private:
  const clang::SourceLocation &Loc;
  clang::DiagnosticsEngine &DE;
  AutocheckWarnings Warning;
  bool ReportWarning = true;
};

class AutocheckDiagnostic {
public:
  AutocheckDiagnostic() = delete;

  template <typename... ArgsType>
  static WarningCounter &
  reportWarning(clang::DiagnosticsEngine &DE, const clang::SourceLocation &Loc,
                AutocheckWarnings Warning, ArgsType &&...Args) {
    // Open a new scope to control when DB gets destructed. The diagnostic is
    // emitted in the DiagnosticBuilder's destructor.
    {
      AutocheckDiagnosticBuilder DB = Diag(DE, Loc, Warning);
      addArgsToDiagBuilder(DB, std::forward<ArgsType>(Args)...);
    }

    return getWarningCounter();
  }

private:
  static AutocheckDiagnosticBuilder Diag(clang::DiagnosticsEngine &DE,
                                         const clang::SourceLocation &Loc,
                                         AutocheckWarnings Warning);

  // Base case for variadic template.
  static void addArgsToDiagBuilder(AutocheckDiagnosticBuilder &DB);

  template <typename ArgType, typename... ArgsType>
  static void addArgsToDiagBuilder(AutocheckDiagnosticBuilder &DB,
                                   ArgType &&Arg, ArgsType &&...Args) {
    DB << std::forward<ArgType>(Arg);
    addArgsToDiagBuilder(DB, std::forward<ArgsType>(Args)...);
  }
};

} // namespace autocheck

#endif
