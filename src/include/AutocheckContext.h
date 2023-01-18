//===- AutocheckContext.h - Mechanism for passing implicit data -----------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// Context for storing and retrieving data that is required throughout the
// program. An instance of this class is created in Autocheck.cpp and passed to
// every class that operates with warnings.
//
//===----------------------------------------------------------------------===//

#ifndef AUTOCHECK_CONTEXT_H
#define AUTOCHECK_CONTEXT_H

#include "Diagnostics/AutocheckWarnings.h"
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace autocheck {

/// This class provides a way to enable, disable and query warnings. An instance
/// of this class is created in Autocheck.cpp and passed to all classes that
/// operate with warnings.
class AutocheckContext {
public:
  bool enableWarning(const std::string &Warning);
  bool isEnabled(AutocheckWarnings Warning) const;

private:
  /// Mapping between command line flags and warning types.
  static const std::unordered_map<std::string, AutocheckWarnings> WarningMap;

  std::unordered_set<AutocheckWarnings> EnabledWarnings;
};

} // namespace autocheck

#endif
