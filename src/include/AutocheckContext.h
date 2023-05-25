//===- AutocheckContext.h - Mechanism for passing implicit data -----------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// Context for storing and retrieving data that is required throughout the
// program. It is implemented as a singleton and should be accessed using
// AutocheckContext::Get().
//
//===----------------------------------------------------------------------===//

#ifndef AUTOCHECK_CONTEXT_H
#define AUTOCHECK_CONTEXT_H

#include "Diagnostics/AutocheckWarnings.h"
#include "llvm/ADT/StringRef.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace autocheck {

/// This class provides a way to enable, disable and query warnings. It is
/// implemented as a singleton and should be accessed using
/// AutocheckContext::Get().
class AutocheckContext {
public:
  static AutocheckContext &Get() {
    static AutocheckContext instance;
    return instance;
  }

  AutocheckContext(const AutocheckContext &Context) = delete;

  bool enableWarning(const std::string &Warning);
  bool isEnabled(AutocheckWarnings Warning) const;
  const std::unordered_set<AutocheckWarnings> &getEnabledWarnings() const;

  // Maximum number of warnings per rule to display. If set to zero, all
  // warnings will be shown.
  unsigned WarningLimit = 0;
  // Information about the presence of option -check-system-headers. Has value
  // true if Autosar rules should be checked in system headers, false otherwise.
  bool CheckSystemHeaders = false;
  // Information about the presence of option -dont-check-headers. Has value
  // true if Autosar rules should not be checked in headers, false otherwise.
  bool DontCheckHeaders = false;
  // Vector of arguments to option -check-between-lines. If empty, this option
  // isn't present.
  std::vector<std::string> CheckBetweenLines;
  // Information about the presence of option -dont-check-macro-expansions. Has
  // value true if Autosar rules should be checked in macro expansions, false
  // otherwise.
  bool DontCheckMacroExpansions = false;
  // Path to output file to write all detected warnings to.
  llvm::StringRef OutputPath;
  // Should output all diagnostics or just the summary.
  bool FullOutput = true;

private:
  AutocheckContext() = default;

private:
  /// Mapping between command line flags and warning types.
  static const std::unordered_map<std::string, AutocheckWarnings> WarningMap;

  std::unordered_set<AutocheckWarnings> EnabledWarnings;
};

} // namespace autocheck

#endif
