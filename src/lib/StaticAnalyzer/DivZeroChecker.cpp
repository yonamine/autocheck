//===--- DivZeroChecker.cpp - Detector for division by zero ---------------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file implements DivZeroChecker, that performs checks for division by
// zero, by an undefined value, and by an unknown value.
//
//===----------------------------------------------------------------------===//

#include "StaticAnalyzer/DivZeroChecker.h"

#include "Diagnostics/AutocheckDiagnostic.h"
#include <algorithm>

namespace autocheck {

void DivZeroChecker::checkPreStmt(const clang::BinaryOperator *B,
                                  clang::ento::CheckerContext &C) const {
  // If std::endl is found in code checkPreStmt will be called
  // several times. Purpose of next few lines is to make sure we only get one
  // warning per line.
  if (std::find(PreviousBinaryOperatorLocations.cbegin(),
                PreviousBinaryOperatorLocations.cend(),
                B->getOperatorLoc()) != PreviousBinaryOperatorLocations.end())
    return;
  PreviousBinaryOperatorLocations.push_back(B->getOperatorLoc());

  clang::BinaryOperator::Opcode Op = B->getOpcode();
  if (Op != clang::BO_Div && Op != clang::BO_Rem && Op != clang::BO_DivAssign &&
      Op != clang::BO_RemAssign)
    return;

  if (!B->getRHS()->getType()->isScalarType())
    return;

  clang::ento::SVal Denom = C.getSVal(B->getRHS());
  llvm::Optional<clang::ento::DefinedSVal> DV =
      Denom.getAs<clang::ento::DefinedSVal>();

  // Divide-by-undefined handled in the generic checking for uses of
  // undefined values.
  if (!DV) {
    AutocheckDiagnostic::reportWarning(
        C.getAnalysisManager().getASTContext().getDiagnostics(),
        B->getRHS()->getExprLoc(), AutocheckWarnings::divByZero);
    return;
  }

  // Check for divide by zero.
  clang::ento::ConstraintManager &CM = C.getConstraintManager();
  clang::ento::ProgramStateRef stateNotZero, stateZero;
  std::tie(stateNotZero, stateZero) = CM.assumeDual(C.getState(), *DV);

  // Definite division by zero.
  if (!stateNotZero) {
    assert(stateZero);
    AutocheckDiagnostic::reportWarning(
        C.getAnalysisManager().getASTContext().getDiagnostics(),
        B->getRHS()->getExprLoc(), AutocheckWarnings::divByZero);
    return;
  }

  // Possible division by zero.
  if (stateNotZero && stateZero) {
    AutocheckDiagnostic::reportWarning(
        C.getAnalysisManager().getASTContext().getDiagnostics(),
        B->getRHS()->getExprLoc(), AutocheckWarnings::divByZero);
    return;
  }

  // If we get here, then the denom should not be zero. We abandon the implicit
  // zero denom case for now.
  C.addTransition(stateNotZero);
}

} // namespace autocheck
