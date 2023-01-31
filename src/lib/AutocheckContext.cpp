//===--- AutocheckContext.cpp - Mechanism for passing implicit data -------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the AutocheckContext class.
//
//===----------------------------------------------------------------------===//

#include "AutocheckContext.h"
#include "Diagnostics/AutocheckWarnings.h"
#include "llvm/ADT/StringRef.h"

namespace autocheck {

const std::unordered_map<std::string, AutocheckWarnings>
    AutocheckContext::WarningMap{
#define DIAG(ENUM, FLAG, MESSAGE, RULE) {FLAG, AutocheckWarnings::ENUM},
#include "Diagnostics/AutocheckWarnings.def"
    };

bool AutocheckContext::enableWarning(const std::string &Warning) {
  if (Warning == "all") {
    for (const auto &WarningItem : WarningMap) {
      EnabledWarnings.insert(WarningItem.second);
    }
    return true;
  }

  bool Disable = false;
  llvm::StringRef W(Warning);
  if (W.starts_with("no-")) {
    Disable = true;
    W = W.slice(3, W.size());
  }
  auto It = WarningMap.find(W.str());
  if (It != WarningMap.end()) {
    if (Disable)
      EnabledWarnings.erase(It->second);
    else
      EnabledWarnings.insert(It->second);
    return true;
  }
  return false;
}

bool AutocheckContext::isEnabled(AutocheckWarnings Warning) const {
  return EnabledWarnings.find(Warning) != EnabledWarnings.end();
}

} // namespace autocheck
