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

// Storage class for diagnostics information.
struct DiagnosticInfo {
  const char *Message;
  const char *Rule;
  clang::DiagnosticIDs::Level Level;
};

// Observer class for diagnostic events.
//
// To be notified when a diagnostic event is emitted extend this class and
// register the object using
// `AutocheckDiagnostic::AddDiagListener`
// When it is no longer needed, unregister it with
// `AutocheckDiagnostic::RemoveDiagListener`
class DiagListener {
public:
  virtual void OnDiagnosticEmitted(AutocheckWarnings Warning,
                                   const clang::SourceLocation &Loc) = 0;
};

// Utility class that keeps track of consecutive warnings of the same type for a
// select group of Autosar rules which emit a lot of warnings in a row. This
// helps declutter warnings reported to the user.
class WarningRepeatChecker {
public:
  // Helper class for storing line numbers of the last two reported diagnostics.
  struct LineNumbers {
    unsigned Current;
    unsigned Previous;

    bool isAlreadyEmitted() { return Current == Previous; };
  };

  static WarningRepeatChecker &getWarningRepeatChecker();
  /// Checks whether the given warning is in the set of rules whose warnings
  /// shouldn't repeat in the same line of a file.
  bool shouldControl(AutocheckWarnings Warning);
  /// Updates the previous line number and returns an object on which
  /// isAlreadyEmitted() can be called.
  /// Assumes that each subsequent call for the same type of warning gets a
  /// SourceLocation whose global line number comes after (or is the same) as
  /// the line number from the previous call.
  LineNumbers updateLineNumber(clang::DiagnosticsEngine &DE,
                               const clang::SourceLocation &Loc,
                               AutocheckWarnings Warning);

private:
  WarningRepeatChecker();

  // Map of previous warning line numbers for a group Autosar rules that
  // WarningRepeatChecker tracks.
  std::unordered_map<AutocheckWarnings, unsigned> PreviousLineNumbers;
};

// Warning counter for each Autosar rule. Used to check if the last emitted
// warning reached or exceeded the maximum number of warnings per rule.
class WarningCounter {
public:
  WarningCounter(const unsigned MaxWarning) : MaxWarning(MaxWarning) {}

  /// Type of the latest warning. Used to check to whom following note
  /// diagnostics belongs.
  AutocheckWarnings Warning;
  bool limitReached() const;
  bool limitExceeded() const;

  friend class AutocheckDiagnosticBuilder;

private:
  // Helper class that stores information whether a diagnostic reached and
  // exceeded the maximum number of warnings per rule.
  struct LimitPair {
    bool LimitExceeded;
    bool LimitReached;
  };

  /// For a given AutocheckWarning, this method:
  /// - sets the corresponding LimitPair by consulting the number of reported
  ///   warnings from the warning counter map
  /// - increments number of reported warnings within the warning counter map
  void setLimitPairAndIncrement(AutocheckWarnings Warning);
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

/// Check if given location is in main file or allowed header based on currently
/// set flags.
bool appropriateHeaderLocation(clang::DiagnosticsEngine &DE,
                               const clang::SourceLocation &Loc);

WarningCounter &getWarningCounter();

/// Custom DiagnosticsBuilder class that checks autocheck diagnostics options to
/// determine whether to report the diagnostic to the user or not.
class AutocheckDiagnosticBuilder : public clang::DiagnosticBuilder {
public:
  AutocheckDiagnosticBuilder(clang::DiagnosticBuilder &DB,
                             clang::DiagnosticsEngine &DE,
                             const clang::SourceLocation &Loc,
                             AutocheckWarnings Warning,
                             clang::DiagnosticIDs::Level Level);

  ~AutocheckDiagnosticBuilder();

private:
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

  static AutocheckWarnings getLatestWarning();

  static bool AddDiagListener(DiagListener *DL);
  static bool RemoveDiagListener(DiagListener *DL);

  static const DiagnosticInfo &GetDiagInfo(AutocheckWarnings Warning);

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
