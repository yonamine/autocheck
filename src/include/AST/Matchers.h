//===--- Matchers.h - Matchers for AUTOSAR rule checks --------------------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file performs checks for AUTOSAR rules by finding pattern matches in the
// clang AST.
//
// This implements the following checks:
// - [A0-1-2]  The value returned by a function having a non-void return type
//             that is not an overloaded operator shall be used.
// - [A12-8-5] A copy assignment and a move assignment operators shall handle
//             self-assignment.
//
//===----------------------------------------------------------------------===//

#ifndef AST_MATCHERS_H
#define AST_MATCHERS_H

#include "AutocheckContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/Diagnostic.h"

namespace autocheck {

class AutocheckDiagnostic;

/// [A0-1-2] The value returned by a function having a non-void return type that
/// is not an overloaded operator shall be used.
namespace UnusedReturnMatcher {
class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
  AutocheckDiagnostic &AD;

public:
  Callback(AutocheckDiagnostic &AD);
  virtual void
  run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
};
clang::ast_matchers::StatementMatcher makeMatcher();
bool isFlagPresent(const AutocheckContext &Context);
} // namespace UnusedReturnMatcher

/// [A12-8-5] A copy assignment and a move assignment operators shall handle
/// self-assignment.
/// Note: This matcher is taken from clang-tidy
namespace SelfAssignmentMatcher {
class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
  AutocheckDiagnostic &AD;

public:
  Callback(AutocheckDiagnostic &AD);
  virtual void
  run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
};
clang::ast_matchers::DeclarationMatcher makeMatcher();
bool isFlagPresent(const AutocheckContext &Context);
} // namespace SelfAssignmentMatcher

} // namespace autocheck

#endif
