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
#include <string>
#include <unordered_map>
#include <unordered_set>

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

private:
  AutocheckContext() = default;

private:
  /// Mapping between command line flags and warning types.
  static const std::unordered_map<std::string, AutocheckWarnings> WarningMap;

  std::unordered_set<AutocheckWarnings> EnabledWarnings;
};

} // namespace autocheck

#endif
