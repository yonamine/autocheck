//===--- SideEffectChecker.h - Expression side effect checker -------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares helper functions to determine whether a given expression
// has side effects according to AUTOSAR rules.
//
//===----------------------------------------------------------------------===//

#ifndef AST_SIDE_EFFECT_CHECKER_H
#define AST_SIDE_EFFECT_CHECKER_H

#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"

namespace autocheck {

bool hasAutosarSideEffects(const clang::Expr *E, const clang::ASTContext &Ctx,
                           bool IncludePossibleEffects = true);

} // namespace autocheck

#endif
