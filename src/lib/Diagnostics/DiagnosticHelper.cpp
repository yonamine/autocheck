//===- DiagnosticHelper.cpp - Helper functions for autocheck diagnostics --===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file implements helper functions for working with autocheck diagnostics.
//
//===----------------------------------------------------------------------===//

#include "Diagnostics/DiagnosticHelper.h"

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

namespace autocheck {

int hashRuleId(const char *RuleId) {
  size_t Length = strlen(RuleId);
  int Hash = 0;
  for (size_t i = 1; i < Length; i++) {
    if (i + 1 < Length && RuleId[i + 1] != '-') {
      Hash = Hash * 100 + (RuleId[i] - '0') * 10 + (RuleId[i + 1] - '0');
      i += 2;
    } else {
      Hash = Hash * 100 + RuleId[i] - '0';
      i++;
    }
  }
  Hash = Hash * 10 + (RuleId[0] == 'A' ? 0 : 1);
  return Hash;
}

struct RuleFlagPair {
  const char *RuleId;
  const char *Flag;
};

void printRuleIdentifierTable() {
  // Table header fields.
  const char *RuleHeader = "Rule ID";
  const char *IdentiferHeader = "Identifier";

  // Sort rules.
  std::vector<std::pair<int, RuleFlagPair>> HashedRules = {
#define DIAG(ENUM, FLAG, MESSAGE, RULE) {hashRuleId(RULE), {RULE, FLAG}},
#define NOTE(ENUM, MESSAGE)
#include "Diagnostics/AutocheckWarnings.def"
  };
  std::sort(
      HashedRules.begin(), HashedRules.end(),
      [](const auto &lhs, const auto &rhs) { return lhs.first < rhs.first; });
  std::vector<RuleFlagPair> SortedRules;
  std::transform(
      HashedRules.cbegin(), HashedRules.cend(), std::back_inserter(SortedRules),
      [](const std::pair<int, RuleFlagPair> Pair) { return Pair.second; });

  // Find max lengths for each field.
  int MaxRuleWidth = std::accumulate(
      SortedRules.cbegin(), SortedRules.cend(), std::strlen(RuleHeader),
      [](int Acc, const RuleFlagPair &Elem) {
        return std::max<int>(Acc, strlen(Elem.RuleId));
      });
  int MaxFlagWidth = std::accumulate(
      SortedRules.cbegin(), SortedRules.cend(), std::strlen(IdentiferHeader),
      [](int Acc, const RuleFlagPair &Elem) {
        return std::max<int>(Acc, strlen(Elem.Flag));
      });

  // Print table.
  std::cout << "| " << std::setw(MaxRuleWidth) << RuleHeader << " | "
            << std::setw(MaxFlagWidth) << IdentiferHeader << " |\n";
  std::cout << std::setfill('-') << "| " << std::setw(MaxRuleWidth) << ""
            << " | " << std::setw(MaxFlagWidth) << ""
            << " |\n"
            << std::setfill(' ');
  std::for_each(SortedRules.cbegin(), SortedRules.cend(),
                [MaxRuleWidth, MaxFlagWidth](const RuleFlagPair DiagInfo) {
                  std::cout << "| " << std::setw(MaxRuleWidth)
                            << DiagInfo.RuleId << " | "
                            << std::setw(MaxFlagWidth) << DiagInfo.Flag
                            << " |\n";
                });
}

} // namespace autocheck
