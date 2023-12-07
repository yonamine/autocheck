//===- DiagnosticHelper.h - Helper functions for autocheck diagnostics ----===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file declares helper functions for working with autocheck diagnostics.
//
//===----------------------------------------------------------------------===//

#ifndef DIAGNOSTICS_DIAGNOSTIC_HELPER_H
#define DIAGNOSTICS_DIAGNOSTIC_HELPER_H

namespace autocheck {

// Transform AUTOSAR rule id to an integer so that it can be sorted.
//
// For example 'A18-1-3' turns into 1801030 (the least significant digit is 0 if
// the rule starts with A or 1 if it starts with M)
int hashRuleId(const char *RuleId);

} // namespace autocheck

#endif
