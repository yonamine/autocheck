//===--- AutocheckContext.cpp - Mechanism for passing implicit data -------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
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
#define NOTE(ENUM, MESSAGE)
#include "Diagnostics/AutocheckWarnings.def"
    };

// Set of analysis based warnings. These are more expensive to check, so we keep
// them separate.
const std::unordered_set<AutocheckWarnings> AnalysisWarnings{
    AutocheckWarnings::divByZero,       AutocheckWarnings::pointerSub,
    AutocheckWarnings::unreachableCode, AutocheckWarnings::recursionUsed,
    AutocheckWarnings::nullDereference,
};

// Set of unused header diagnostics. These all perform the same check, just in
// different ways. At most one should be active at a time.
const std::unordered_set<AutocheckWarnings> HeaderUnusedWarnings{
    AutocheckWarnings::headersUnused, AutocheckWarnings::headersUnusedSystemOff,
    AutocheckWarnings::headersUnusedSystemSmart};

bool AutocheckContext::enableWarning(const std::string &Warning) {
  if (Warning == "all") {
    for (const auto &WarningItem : WarningMap) {
      // Only turn on default version of headersUnused.
      if (WarningItem.second == AutocheckWarnings::headersUnusedSystemOff ||
          WarningItem.second == AutocheckWarnings::headersUnusedSystemSmart)
        continue;

      // Skip analysis warnings.
      if (AnalysisWarnings.find(WarningItem.second) != AnalysisWarnings.cend())
        continue;

      EnabledWarnings.insert(WarningItem.second);
    }
    return true;
  }

  if (Warning == "analysis") {
    EnabledWarnings.insert(AnalysisWarnings.cbegin(), AnalysisWarnings.cend());
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
    if (Disable) {
      EnabledWarnings.erase(It->second);
    } else {
      // At most one headersUnused option can be enabled at a time.
      if (HeaderUnusedWarnings.find(It->second) != HeaderUnusedWarnings.end()) {
        std::for_each(HeaderUnusedWarnings.cbegin(), HeaderUnusedWarnings.cend(), [this](AutocheckWarnings W){
          const auto It = EnabledWarnings.find(W);
          if (It != EnabledWarnings.cend())
            EnabledWarnings.erase(It);
        });
      }
      EnabledWarnings.insert(It->second);
    }
    return true;
  }
  return false;
}

bool AutocheckContext::isEnabled(AutocheckWarnings Warning) const {
  return EnabledWarnings.find(Warning) != EnabledWarnings.end();
}

const std::unordered_set<AutocheckWarnings> &AutocheckContext::getEnabledWarnings() const {
  return EnabledWarnings;
}

} // namespace autocheck
