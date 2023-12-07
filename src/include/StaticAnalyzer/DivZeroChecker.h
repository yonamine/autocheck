//===--- DivZeroChecker.h - Detector for division by zero -----------------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file defines DivZeroChecker, that performs checks for division by zero,
// by an undefined value, and by an unknown value.
//
// [A5-6-1] The right hand operand of the integer division or remainder
// operators shall not be equal to zero.
//
//===----------------------------------------------------------------------===//

#ifndef STATIC_ANALYZER_DIV_ZERO_CHECKER_H
#define STATIC_ANALYZER_DIV_ZERO_CHECKER_H

#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"

namespace autocheck {

class DivZeroChecker : public clang::ento::Checker<
                           clang::ento::check::PreStmt<clang::BinaryOperator>> {
  mutable std::vector<clang::SourceLocation> PreviousBinaryOperatorLocations;
  mutable std::unique_ptr<clang::ento::BuiltinBug> BT;

public:
  void checkPreStmt(const clang::BinaryOperator *B,
                    clang::ento::CheckerContext &C) const;
};

} // namespace autocheck

#endif
