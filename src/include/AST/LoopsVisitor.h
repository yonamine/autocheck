//===--- LoopsVisitor.h - Visitor for loop checks -------------------------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file performs checks for rules related to loops by traversing the clang
// AST.
//
// This implements the following checks:
// - [A6-5-1] A for-loop that loops through all elements of the container and
//            does not use its loop-counter shall not be used.
// - [A6-5-2] A for loop shall contain a single loop-counter which shall not
//            have floating-point type.
// - [M6-5-2] If loop-counter is not modified by -- or ++, then, within
//            condition, the loop-counter shall only be used as an operand to
//            <=, <, > or >=.
// - [M6-5-3] The loop-counter shall not be modified within condition or
//            statement.
// - [A6-5-4] For-init-statement and expression should not perform actions other
//            than loop-counter initialization and modification.
// - [M6-5-4] The loop-counter shall be modified by one of: --, ++, -= n, or
//            += n; where n remains constant for the duration of the loop.
// - [M6-5-5] A loop-control-variable other than the loop-counter shall not be
//            modified within condition or expression.
// - [M6-5-6] A loop-control-variable other than the loop-counter which is
//            modified in statement shall have type bool.
// - [M6-6-3] The continue statement shall only be used within a well-formed for
//            loop.
//
//===----------------------------------------------------------------------===//

#ifndef AST_LOOPS_VISITOR_H
#define AST_LOOPS_VISITOR_H

#include "AutocheckContext.h"
#include "clang/AST/Expr.h"
#include "clang/AST/RecursiveASTVisitor.h"

namespace autocheck {

class AutocheckDiagnostic;

/// Storage class for types of usage of a variable.
class Usage {
public:
  Usage(bool N = true, bool D1 = false, bool D2 = false)
      : Normal(N), DerefL1(D1), DerefL2(D2) {}
  bool hasNormalUsage() const { return Normal; }
  bool hasDerefUsageL1() const { return DerefL1; }
  bool hasDerefUsageL2() const { return DerefL2; }
  bool hasDerefUsage() const { return DerefL1 || DerefL2; }

  void setNormalUsage(bool N) { Normal = N; }
  void setDerefUsageL1(bool DL1) { DerefL1 = DL1; }
  void setDerefUsageL2(bool DL2) { DerefL2 = DL2; }

  bool operator==(const Usage &U) const {
    return (Normal == U.Normal && DerefL1 == U.DerefL1 && DerefL2 == U.DerefL2);
  }

private:
  bool Normal;
  bool DerefL1;
  bool DerefL2;
};

using VarDeclPtrUsage = std::pair<const clang::VarDecl *, Usage>;
using VarDeclPtrMap = std::map<const clang::VarDecl *, Usage>;

/// This visitor extracts variables from a given statement or expression.
class VarExtractor : public clang::RecursiveASTVisitor<VarExtractor> {
  /// We do not need to worry about duplicates because we are using
  /// the SmallPtrMap container.
  VarDeclPtrMap &ExtractedVars;
  llvm::SmallVector<bool, 4> InsideDerefOp;

public:
  explicit VarExtractor(VarDeclPtrMap &ExtractedVars);
  bool TraverseStmt(clang::Stmt *S);

  bool VisitDeclRefExpr(clang::DeclRefExpr *DRE);
  bool VisitVarDecl(clang::VarDecl *VD);
};

/// This visitor extracts modified variables from a given statement or
/// expression.
class ModifiedVarExtractor
    : public clang::RecursiveASTVisitor<ModifiedVarExtractor> {
  /// We do not need to worry about duplicates because we are using the
  /// SmallPtrMap container.
  VarDeclPtrMap *ControlVariables;
  VarDeclPtrMap &CounterCandidates;
  bool IgnoreControlVariables = false;
  uint8_t NumVarsInsideIncDec{0};
  enum DerefLevel { Normal, DerefL1, DerefL2 };
  llvm::SmallVector<bool, 4> InsideAssignmentOp;
  llvm::SmallVector<bool, 4> InsideIncrementOp;
  llvm::SmallVector<bool, 4> InsideArraySubscriptOp;
  llvm::SmallVector<bool, 4> InsideDerefOp;
  void SetControlVariablesUsage(const clang::VarDecl *VD,
                                enum DerefLevel Level);
  bool IsDerefUsageL1() const;
  bool IsDerefUsageL2() const;
  bool IsNormalUsage() const;
  bool InsideIncOrAss() const;
  bool PopInDeref();
  void PopInDeref(bool b);
  void PushInDeref(bool b);

public:
  explicit ModifiedVarExtractor(VarDeclPtrMap *ControlVariables,
                                VarDeclPtrMap &CounterCandidates);
  explicit ModifiedVarExtractor(VarDeclPtrMap &CounterCandidates);

  bool TraverseStmt(clang::Stmt *S);
  bool VisitDeclRefExpr(clang::DeclRefExpr *DRE);
  bool VisitCallExpr(clang::CallExpr *CE);
};

/// This visitor checks if ill-formed loop has continue statement.
class ContinueStmtChecker
    : public clang::RecursiveASTVisitor<ContinueStmtChecker> {
  llvm::SmallVector<const clang::Stmt *, 4> ParentVector;
  const clang::Stmt *CurrentLoop;
  llvm::SmallVector<bool, 4> InsideDerefOp;

public:
  explicit ContinueStmtChecker(const clang::ForStmt *FS);

  bool TraverseStmt(clang::Stmt *S);
  bool VisitContinueStmt(clang::ContinueStmt *CS);
};

/// This visitor checks if variables are modified using increment or
/// decrement operations.
class IncDecModifiedVars
    : public clang::RecursiveASTVisitor<IncDecModifiedVars> {
  VarDeclPtrMap &ModifiedVariables;
  bool InsideIncDecOp;

public:
  explicit IncDecModifiedVars(VarDeclPtrMap &ModifiedVariables);

  bool TraverseStmt(clang::Stmt *S);
  bool VisitDeclRefExpr(clang::DeclRefExpr *DRE);
};

/// This visitor checks if variables are compared using relational operators.
class RelOpVarExtractor : public clang::RecursiveASTVisitor<RelOpVarExtractor> {
  VarDeclPtrMap &Variables;
  bool InsideRelOp;

public:
  explicit RelOpVarExtractor(VarDeclPtrMap &Variables);

  bool TraverseStmt(clang::Stmt *S);
  bool VisitDeclRefExpr(clang::DeclRefExpr *DRE);
};

/// This visitor checks if there are forbidden actions inside for loop.
class ForbiddenActVisitor
    : public clang::RecursiveASTVisitor<ForbiddenActVisitor> {
public:
  bool VisitUnaryOperator(clang::UnaryOperator *UO);
};

/// This visitor checks does for loop loops through all elements
/// of the container and does not use loop-counter.
class ContainerVisitor : public clang::RecursiveASTVisitor<ContainerVisitor> {
  clang::ASTContext &AC;
  const clang::ForStmt *ParentFS;
  VarDeclPtrMap Counters;

public:
  explicit ContainerVisitor(clang::ASTContext &AC, clang::ForStmt *ParentFS,
                            VarDeclPtrMap &Counters);

  const clang::Stmt *isForStmtParent(const clang::Stmt *S);
  /// Checks if S is an operand of an additive (+, -) or multiplicative (*, /)
  /// operator.
  const clang::BinaryOperator *isStmtInsideAddOrMulOp(const clang::Stmt *S);
  /// Checks if E evaluates to the neutral element to an additive (+, -) or
  /// multiplicative (*, /) operator.
  bool isOperandNeutralForOp(const clang::Expr *E, bool IsMultiplicativeOp);
  const clang::Stmt *isOperandCounterVariable(const clang::Stmt *S);
  bool VisitCXXMemberCallExpr(clang::CXXMemberCallExpr *CMCE);
  bool VisitVarDecl(clang::VarDecl *VD);
  bool VisitUnaryOperator(const clang::UnaryOperator *UO);
};

/// This visitor checks if init part of a for loop is evaluatable to zero,
/// so that it loops through all elements.
class ContainerVisitorInit
    : public clang::RecursiveASTVisitor<ContainerVisitorInit> {
  clang::ASTContext &AC;

public:
  explicit ContainerVisitorInit(clang::ASTContext &AC);
  bool VisitBinaryOperator(const clang::BinaryOperator *BO);
};

/// This visitor checks whether loop-counters marked as used are really used.
class UsedContainerVisitor
    : public clang::RecursiveASTVisitor<UsedContainerVisitor> {
  clang::ASTContext &AC;
  const clang::ForStmt *ParentFS;
  VarDeclPtrMap Counters;

public:
  explicit UsedContainerVisitor(clang::ASTContext &AC, clang::ForStmt *ParentFS,
                                VarDeclPtrMap &Counters);

  bool compareToArraySize(const clang::Expr *E, const uint64_t ArraySize);
  bool VisitArraySubscriptExpr(clang::ArraySubscriptExpr *ASE);
  bool VisitDeclRefExpr(clang::DeclRefExpr *DRE);
};

/// This visitor checks if loop-counter is ill-modified.
class ModifiedCntVisitor
    : public clang::RecursiveASTVisitor<ModifiedCntVisitor> {
  clang::ASTContext &AC;
  const clang::Stmt *ParentForStmt;
  VarDeclPtrMap Counters;
  VarDeclPtrMap ModifiedVars;

public:
  explicit ModifiedCntVisitor(clang::ASTContext &AC,
                              const clang::Stmt *ParentForStmt,
                              VarDeclPtrMap &Counters,
                              VarDeclPtrMap &ModifiedVars);

  const clang::Stmt *isForStmtParent(const clang::Stmt *S);
  const clang::Stmt *isIncrementDecrementOperatorParent(const clang::Stmt *S);
  const clang::Stmt *isCompoundAssignmentOpParent(const clang::Stmt *S);
  bool VisitDeclRefExpr(clang::DeclRefExpr *DRE);
};

/// This visitor checks if an assigment operator within Init part of a loop is
/// of correct form (namely, just an initialization with constant or a variable,
/// no expressions or function calls are expected).
class CheckAssignment : public clang::RecursiveASTVisitor<CheckAssignment> {
  bool &Correct;

public:
  explicit CheckAssignment(bool &C);
  bool VisitBinaryOperator(const clang::BinaryOperator *BO);
};

/// Main visitor for loop related rules.
class ForLoopVisitor : public clang::RecursiveASTVisitor<ForLoopVisitor> {
  AutocheckDiagnostic &AD;
  clang::ASTContext &AC;
  
  VarDeclPtrMap ControlVariables;
  VarDeclPtrMap CounterVariables;
  VarDeclPtrMap NonCounterVariables;

  bool WarnSingleLoopCounterFloatType = false;
  bool WarnNonLoopCounterBool = false;
  bool WarnContinueStmtWellFormedLoop = false;
  bool WarnNonLoopCounterModified = false;
  bool WarnLoopCounterModified = false;
  bool WarnCondExprIncDecLoop = false;
  bool WarnInitIncPartsLoopCounter = false;
  bool WarnContainerLoopIllFormed = false;
  bool WarnLoopCounterIllModified = false;

public:
  explicit ForLoopVisitor(AutocheckDiagnostic &AD, clang::ASTContext &AC);

  void run(clang::TranslationUnitDecl *TUD);
  bool TraverseDecl(clang::Decl *D);

  bool VisitForStmt(clang::ForStmt *FS);
  void classifyLoopVariables(clang::ForStmt *FS);

  /// [A6-5-2] A for loop shall contain a single loop-counter which shall not
  /// have floating-point type.
  bool checkNumberOfCountersAndTypes(clang::ForStmt *FS);

  /// [M6-5-6] A loop-control-variable other than the loop-counter which is
  /// modified in statement shall have type bool.
  bool checkNonCountersType(clang::ForStmt *FS);

  /// [M6-6-3] The continue statement shall only be used within a well-formed
  /// for loop.
  void checkIsLoopWithContinueWellFormed(clang::ForStmt *FS, bool LS);

  /// [M6-5-5] A loop-control-variable other than the loop-counter shall not be
  /// modified within condition or expression.
  void checkIsNonCounterIllModified(clang::ForStmt *FS);

  /// [M6-5-3] The loop-counter shall not be modified within condition or
  /// statement.
  void checkIsCounterIllModified(clang::ForStmt *FS);

  /// [M6-5-2] If loop-counter is not modified by -- or ++, then, within
  /// condition, the loop-counter shall only be used as an operand to <=, <, >
  /// or >=.
  void checkIsCounterIncorrectlyUsed(clang::ForStmt *FS);

  /// [A6-5-4] For-init-statement and expression should not perform actions
  /// other than loop-counter initialization and modification.
  void checkLoopActions(clang::ForStmt *FS);

  /// [A6-5-1] A for-loop that loops through all elements of the container and
  /// does not use its loop-counter shall not be used.
  void checkContainerLoop(clang::ForStmt *FS);

  /// [M6-5-4] The loop-counter shall be modified by one of: --, ++, -= n, or
  /// += n; where n remains constant for the duration of the loop.
  void isCounterCorrectlyModified(clang::ForStmt *FS);
};

} // namespace autocheck

#endif
