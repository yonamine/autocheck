//===- DiagnosticHelper.cpp - Helper functions for autocheck diagnostics --===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
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

} // namespace autocheck
