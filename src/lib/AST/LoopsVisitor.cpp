//===--- LoopsVisitor.cpp - Visitor for loop related checks ---------------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file implements the ForLoopVisitor class and all helper visitors that
// check loops.
//
//===----------------------------------------------------------------------===//

#include "AST/LoopsVisitor.h"

#include "Diagnostics/AutocheckDiagnostic.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/ADT/SmallSet.h"
#include <set>

namespace autocheck {

// Helper functions.
static const clang::VarDecl *isVarDecl(const clang::Decl *D) {
  return llvm::dyn_cast_if_present<const clang::VarDecl>(D);
}

static const clang::BinaryOperator *isBinaryOperator(const clang::Stmt *S) {
  return llvm::dyn_cast_if_present<const clang::BinaryOperator>(S);
}

static const clang::UnaryOperator *isUnaryOperator(const clang::Stmt *S) {
  return llvm::dyn_cast_if_present<const clang::UnaryOperator>(S);
}

static const clang::DeclRefExpr *isDeclRefExpr(const clang::Stmt *S) {
  return llvm::dyn_cast_if_present<const clang::DeclRefExpr>(S);
}

static const clang::IntegerLiteral *isIntegerLiteral(const clang::Expr *E) {
  return llvm::dyn_cast_if_present<const clang::IntegerLiteral>(E);
}

static const clang::FloatingLiteral *isFloatingLiteral(const clang::Expr *E) {
  return llvm::dyn_cast_if_present<const clang::FloatingLiteral>(E);
}

static const clang::Expr *IsDeref(const clang::Expr *E) {
  if (const clang::UnaryOperator *UO =
          llvm::dyn_cast_if_present<clang::UnaryOperator>(E))
    if (UO->getOpcode() == clang::UO_Deref)
      return UO;
  return nullptr;
}

static void findVarDeclSetIntersection(const VarDeclPtrMap &A,
                                       const VarDeclPtrMap &B,
                                       VarDeclPtrMap &R) {
  for (VarDeclPtrUsage VDU : A) {
    if (std::find_if(B.begin(), B.end(), [VDU](const VarDeclPtrUsage &VDUsage) {
          return (VDU.first == VDUsage.first) && (VDU.second == VDUsage.second);
        }) != B.end())
      R.insert(VDU);
  }
}

static void findVarDeclSetDifference(const VarDeclPtrMap &A,
                                     const VarDeclPtrMap &B, VarDeclPtrMap &R) {
  for (VarDeclPtrUsage VDU : A) {
    if (std::find_if(B.begin(), B.end(), [VDU](const VarDeclPtrUsage &VDUsage) {
          return (VDU.first == VDUsage.first) && (VDU.second == VDUsage.second);
        }) == B.end())
      R.insert(VDU);
  }
}

/* Implementation of VarExtractor */

VarExtractor::VarExtractor(VarDeclPtrMap &ExtractedVars)
    : ExtractedVars(ExtractedVars) {
  InsideDerefOp.push_back(false);
}

bool VarExtractor::VisitDeclRefExpr(clang::DeclRefExpr *DRE) {
  if (const clang::VarDecl *VD = isVarDecl(DRE->getDecl())) {
    if (InsideDerefOp.back() == true) {
      ExtractedVars[VD].setDerefUsageL1(true);
      if (InsideDerefOp[InsideDerefOp.size() - 2] == true)
        ExtractedVars[VD].setDerefUsageL2(true);
    } else
      ExtractedVars[VD].setNormalUsage(true);
  }
  return true;
}

bool VarExtractor::TraverseStmt(clang::Stmt *S) {
  if (clang::UnaryOperator *UO =
          llvm::dyn_cast_if_present<clang::UnaryOperator>(S)) {
    if (IsDeref(UO))
      InsideDerefOp.push_back(true);
    clang::RecursiveASTVisitor<VarExtractor>::TraverseStmt(UO->getSubExpr());
    if (IsDeref(UO))
      InsideDerefOp.pop_back();
  }
  if (clang::ArraySubscriptExpr *ASE =
          llvm::dyn_cast_if_present<clang::ArraySubscriptExpr>(S)) {
    InsideDerefOp.push_back(true);
    clang::RecursiveASTVisitor<VarExtractor>::TraverseStmt(ASE->getLHS());
    InsideDerefOp.pop_back();
    clang::RecursiveASTVisitor<VarExtractor>::TraverseStmt(ASE->getRHS());
  }

  clang::RecursiveASTVisitor<VarExtractor>::TraverseStmt(S);
  return true;
}

bool VarExtractor::VisitVarDecl(clang::VarDecl *VD) {
  if (!ExtractedVars.count(VD))
    ExtractedVars[VD] = Usage();
  return true;
}

/* Implementation of ModifiedVarExtractor */

ModifiedVarExtractor::ModifiedVarExtractor(VarDeclPtrMap *ControlVariables,
                                           VarDeclPtrMap &CounterCandidates)
    : ControlVariables(ControlVariables), CounterCandidates(CounterCandidates) {
  InsideAssignmentOp.push_back(false);
  InsideIncrementOp.push_back(false);
  InsideArraySubscriptOp.push_back(false);
  InsideDerefOp.push_back(false);
}

ModifiedVarExtractor::ModifiedVarExtractor(VarDeclPtrMap &CounterCandidates)
    : ControlVariables(nullptr), CounterCandidates(CounterCandidates),
      IgnoreControlVariables(true) {
  InsideAssignmentOp.push_back(false);
  InsideIncrementOp.push_back(false);
  InsideArraySubscriptOp.push_back(false);
  InsideDerefOp.push_back(false);
}

bool ModifiedVarExtractor::PopInDeref() {
  if (InsideDerefOp.back()) {
    InsideDerefOp.pop_back();
    return true;
  }
  return false;
}

void ModifiedVarExtractor::PopInDeref(bool b) {
  if (b)
    InsideDerefOp.pop_back();
}

void ModifiedVarExtractor::PushInDeref(bool b) {
  if (b)
    InsideDerefOp.push_back(true);
}

bool ModifiedVarExtractor::TraverseStmt(clang::Stmt *S) {
  if (const clang::BinaryOperator *BO = isBinaryOperator(S)) {
    if (BO->isAssignmentOp() || BO->isShiftAssignOp() ||
        BO->isCompoundAssignmentOp()) {
      InsideAssignmentOp.push_back(true);
      if (llvm::dyn_cast_if_present<clang::ArraySubscriptExpr>(BO->getLHS()))
        InsideArraySubscriptOp.push_back(true);
      PushInDeref(IsDeref(BO->getLHS()));
      clang::RecursiveASTVisitor<ModifiedVarExtractor>::TraverseStmt(
          BO->getLHS());
      InsideAssignmentOp.pop_back();
      if (llvm::dyn_cast_if_present<clang::ArraySubscriptExpr>(BO->getLHS()))
        InsideArraySubscriptOp.pop_back();
      PopInDeref(IsDeref(BO->getLHS()));
      clang::RecursiveASTVisitor<ModifiedVarExtractor>::TraverseStmt(
          BO->getRHS());
      return true;
    }
  } else if (const clang::UnaryOperator *UO = isUnaryOperator(S)) {
    if (UO->isIncrementDecrementOp()) {
      InsideIncrementOp.push_back(true);

      if (llvm::dyn_cast_if_present<clang::ArraySubscriptExpr>(
              UO->getSubExpr())) {
        InsideArraySubscriptOp.push_back(true);
        PushInDeref(true);
      }

      bool InDerefL1 = PopInDeref();
      bool InDerefL2 = PopInDeref();
      clang::RecursiveASTVisitor<ModifiedVarExtractor>::TraverseStmt(
          UO->getSubExpr());
      PushInDeref(InDerefL2);
      PushInDeref(InDerefL1);

      if (llvm::dyn_cast_if_present<clang::ArraySubscriptExpr>(
              UO->getSubExpr())) {
        InsideArraySubscriptOp.pop_back();
        PopInDeref(true);
      }

      InsideIncrementOp.pop_back();
      return true;
    } else if (IsDeref(UO)) {
      PushInDeref(true);
      clang::RecursiveASTVisitor<ModifiedVarExtractor>::TraverseStmt(
          UO->getSubExpr());
      PopInDeref();
      return true;
    }
  }

  clang::RecursiveASTVisitor<ModifiedVarExtractor>::TraverseStmt(S);
  return true;
}

void ModifiedVarExtractor::SetControlVariablesUsage(const clang::VarDecl *VD,
                                                    enum DerefLevel Level) {
  if (IgnoreControlVariables)
    return;
  if (Level == Normal)
    (*ControlVariables)[VD].setNormalUsage(true);
  if (Level == DerefL1)
    (*ControlVariables)[VD].setDerefUsageL1(true);
  if (Level == DerefL2)
    (*ControlVariables)[VD].setDerefUsageL2(true);
}

bool ModifiedVarExtractor::IsDerefUsageL1() const {
  return InsideDerefOp.back() || InsideArraySubscriptOp.back();
}

bool ModifiedVarExtractor::IsDerefUsageL2() const {
  return (InsideDerefOp.back() && InsideDerefOp[InsideDerefOp.size() - 2]) ||
         (InsideDerefOp.back() && InsideArraySubscriptOp.back()) ||
         (InsideArraySubscriptOp.back() &&
          InsideArraySubscriptOp[InsideArraySubscriptOp.size() - 2]);
}

bool ModifiedVarExtractor::IsNormalUsage() const {
  return !InsideDerefOp.back() && !InsideArraySubscriptOp.back();
}

bool ModifiedVarExtractor::InsideIncOrAss() const {
  return InsideIncrementOp.back() || InsideAssignmentOp.back();
}

bool ModifiedVarExtractor::VisitDeclRefExpr(clang::DeclRefExpr *DRE) {
  if (const clang::VarDecl *VD = isVarDecl(DRE->getDecl())) {
    if (InsideIncOrAss()) {
      if (InsideIncrementOp.back())
        NumVarsInsideIncDec++;
      if (!(InsideArraySubscriptOp.back() && (NumVarsInsideIncDec > 1))) {
        if (IsDerefUsageL2()) {
          CounterCandidates[VD].setDerefUsageL2(true);
          SetControlVariablesUsage(VD, DerefL2);
        }
        if (IsDerefUsageL1()) {
          CounterCandidates[VD].setDerefUsageL1(true);
          SetControlVariablesUsage(VD, DerefL1);
        }
        if (IsNormalUsage()) {
          CounterCandidates[VD].setNormalUsage(true);
          SetControlVariablesUsage(VD, Normal);
        }
      }
    }

    if (!IgnoreControlVariables) {
      (*ControlVariables)[VD].setNormalUsage(true);
    }
  }
  return true;
}

bool ModifiedVarExtractor::VisitCallExpr(clang::CallExpr *CE) {
  if (const clang::FunctionDecl *FD = CE->getDirectCallee()) {
    for (unsigned i = 0; i < FD->getNumParams(); ++i) {
      const clang::ParmVarDecl *Param = FD->getParamDecl(i);
      const clang::QualType &ParamType = Param->getType();
      // Handle arguments of pointer type.
      if ((ParamType->isPointerType() &&
           !(ParamType->getPointeeType().isConstQualified()))) {
        const clang::Expr *E = (CE->getArg(i))->IgnoreParenCasts();
        if (const clang::UnaryOperator *UO = isUnaryOperator(E))
          if (UO->getOpcode() == clang::UO_AddrOf)
            if (const clang::DeclRefExpr *DRE = isDeclRefExpr(UO->getSubExpr()))
              if (const clang::VarDecl *VD = isVarDecl(DRE->getDecl()))
                CounterCandidates[VD].setNormalUsage(true);
        // Handle arguments of reference type.
      } else if (ParamType->isReferenceType() &&
                 !(ParamType.getNonReferenceType().isConstQualified())) {
        const clang::Expr *E = (CE->getArg(i))->IgnoreParenCasts();
        if (const clang::DeclRefExpr *DRE = isDeclRefExpr(E))
          if (const clang::VarDecl *VD = isVarDecl(DRE->getDecl()))
            CounterCandidates[VD].setNormalUsage(true);
      }
    }
  }
  return true;
}

/* Implementation of ContinueStmtChecker */

ContinueStmtChecker::ContinueStmtChecker(const clang::ForStmt *FS)
    : CurrentLoop(FS) {
  ParentVector.push_back(FS);
}

bool ContinueStmtChecker::TraverseStmt(clang::Stmt *S) {
  bool IteratorStmtSeen = false;
  if (llvm::dyn_cast_if_present<const clang::ForStmt>(S) ||
      llvm::dyn_cast_if_present<const clang::WhileStmt>(S) ||
      llvm::dyn_cast_if_present<const clang::DoStmt>(S)) {
    IteratorStmtSeen = true;
    ParentVector.push_back(S);
  }

  if (!clang::RecursiveASTVisitor<ContinueStmtChecker>::TraverseStmt(S))
    return false;

  if (IteratorStmtSeen)
    ParentVector.pop_back();

  return true;
}

bool ContinueStmtChecker::VisitContinueStmt(clang::ContinueStmt *CS) {
  if (CurrentLoop == ParentVector.back())
    return false;

  return true;
}

/* Implementation of IncDecModifiedVars */

IncDecModifiedVars::IncDecModifiedVars(VarDeclPtrMap &ModifiedVariables)
    : ModifiedVariables(ModifiedVariables), InsideIncDecOp(false) {}

bool IncDecModifiedVars::TraverseStmt(clang::Stmt *S) {
  if (const clang::UnaryOperator *UO = isUnaryOperator(S)) {
    if (UO->isIncrementDecrementOp()) {
      InsideIncDecOp = true;
      clang::Expr *SE = UO->getSubExpr();
      clang::RecursiveASTVisitor<IncDecModifiedVars>::TraverseStmt(SE);
      InsideIncDecOp = false;
      return true;
    }
  }

  return clang::RecursiveASTVisitor<IncDecModifiedVars>::TraverseStmt(S);
}

bool IncDecModifiedVars::VisitDeclRefExpr(clang::DeclRefExpr *DRE) {
  if (InsideIncDecOp)
    if (const clang::VarDecl *VD = isVarDecl(DRE->getDecl())) {
      if (!ModifiedVariables.count(VD))
        ModifiedVariables[VD].setNormalUsage(true);
    }
  return true;
}

/* Implementation of RelOpVarExtractor */

RelOpVarExtractor::RelOpVarExtractor(VarDeclPtrMap &Variables)
    : Variables(Variables), InsideRelOp(false) {}

bool RelOpVarExtractor::TraverseStmt(clang::Stmt *S) {
  if (const clang::BinaryOperator *BO = isBinaryOperator(S)) {
    if (BO->isRelationalOp()) {
      InsideRelOp = true;
      clang::RecursiveASTVisitor<RelOpVarExtractor>::TraverseStmt(BO->getLHS());
      clang::RecursiveASTVisitor<RelOpVarExtractor>::TraverseStmt(BO->getRHS());
      InsideRelOp = false;
      return true;
    }
  }

  return clang::RecursiveASTVisitor<RelOpVarExtractor>::TraverseStmt(S);
}

bool RelOpVarExtractor::VisitDeclRefExpr(clang::DeclRefExpr *DRE) {
  if (InsideRelOp)
    if (const clang::VarDecl *VD = isVarDecl(DRE->getDecl()))
      Variables[VD].setNormalUsage(true);
  return true;
}

/* Implementation of ForbiddenActVisitor */

bool ForbiddenActVisitor::VisitUnaryOperator(clang::UnaryOperator *UO) {
  if (UO->isIncrementDecrementOp())
    return false;
  if (UO->getOpcode() == clang::UO_Deref)
    return false;

  return true;
}

/* Implementation of ContainerVisitor */

ContainerVisitor::ContainerVisitor(clang::ASTContext &AC,
                                   clang::ForStmt *ParentFS,
                                   VarDeclPtrMap &Counters)
    : AC(AC), ParentFS(ParentFS), Counters(Counters) {}

const clang::Stmt *ContainerVisitor::isForStmtParent(const clang::Stmt *S) {
  const auto &Parents = AC.getParents(*S);
  assert((Parents.size() > 0) && "At least one parent node expected.");

  // Skip ParenExpr nodes.
  if (const clang::ParenExpr *PE = Parents[0].get<clang::ParenExpr>())
    return isForStmtParent(PE);

  if (const clang::ForStmt *FS = Parents[0].get<clang::ForStmt>())
    if (FS == ParentFS)
      return FS;

  return nullptr;
}

const clang::BinaryOperator *
ContainerVisitor::isStmtInsideAddOrMulOp(const clang::Stmt *S) {
  const auto &Parents = AC.getParents(*S);
  assert((Parents.size() > 0) && "At least one parent node expected.");

  if (const clang::BinaryOperator *BO = Parents[0].get<clang::BinaryOperator>())
    if (BO->isAdditiveOp() || BO->isMultiplicativeOp())
      return BO;

  return nullptr;
}

bool ContainerVisitor::isOperandNeutralForOp(const clang::Expr *E,
                                             bool IsMultiplicativeOp) {
  if (E->isEvaluatable(AC)) {
    int EvaluatedValue = E->EvaluateKnownConstInt(AC).getExtValue();
    if (IsMultiplicativeOp) {
      if (EvaluatedValue != 1)
        return false;
    } else {
      if (EvaluatedValue != 0)
        return false;
    }
  }
  return true;
}

const clang::Stmt *
ContainerVisitor::isOperandCounterVariable(const clang::Stmt *S) {
  const auto &Parents = AC.getParents(*S);
  assert((Parents.size() > 0) && "At least one parent node expected.");

  // Skip ImplicitCastExpr nodes.
  if (const clang::ImplicitCastExpr *ICE =
          Parents[0].get<clang::ImplicitCastExpr>())
    return isOperandCounterVariable(ICE);

  // Skip ParenExpr nodes.
  if (const clang::ParenExpr *PE = Parents[0].get<clang::ParenExpr>()) {
    return isOperandCounterVariable(PE);
  }

  // Try to find counter variable.
  if (const clang::BinaryOperator *BO =
          Parents[0].get<clang::BinaryOperator>()) {
    if (BO->isRelationalOp() || BO->isEqualityOp()) {
      const clang::Expr *LHS = BO->getLHS()->IgnoreParenCasts();
      const clang::Expr *RHS = BO->getRHS()->IgnoreParenCasts();
      if (const clang::DeclRefExpr *DRE = isDeclRefExpr(LHS)) {
        if (const clang::VarDecl *VD = isVarDecl(DRE->getDecl()))
          if (Counters.count(VD))
            if (isForStmtParent(BO))
              return S;
      } else if (const clang::DeclRefExpr *DRE = isDeclRefExpr(RHS)) {
        if (const clang::VarDecl *VD = isVarDecl(DRE->getDecl()))
          if (Counters.count(VD))
            if (isForStmtParent(BO))
              return S;
      }
    }
  }
  return nullptr;
}

bool ContainerVisitor::VisitCXXMemberCallExpr(clang::CXXMemberCallExpr *CMCE) {
  if (const clang::FunctionDecl *FD = CMCE->getDirectCallee()) {
    // Check if the call is to a "size" method.
    const clang::DeclarationNameInfo &DNI = FD->getNameInfo();
    const clang::SourceLocation &SL = DNI.getLoc();
    if ((DNI.getAsString() == "size") &&
        AC.getSourceManager().isInSystemHeader(SL)) {
      // Check if the call to size method is an operand of +, -, * or /
      // binary operator.
      const clang::BinaryOperator *BO = isStmtInsideAddOrMulOp(CMCE);
      if (BO) {
        bool IsMultiplicativeOp = BO->isMultiplicativeOp();
        // Check if the other operand (beside call to size method) is not the
        // neutral element of the binary operation, but is a counter variable.
        // In that case the rule doesn't apply so return early.
        if (BO->getLHS() == CMCE) {
          if (!isOperandNeutralForOp(BO->getRHS(), IsMultiplicativeOp))
            if (isOperandCounterVariable(BO))
              return false;
        } else if (BO->getRHS() == CMCE) {
          if (!isOperandNeutralForOp(BO->getLHS(), IsMultiplicativeOp))
            if (isOperandCounterVariable(BO))
              return false;
        }
      }
    }
  }
  return true;
}

bool ContainerVisitor::VisitVarDecl(clang::VarDecl *VD) {
  if (!Counters.count(VD))
    return true;

  const clang::Expr *Init = VD->getInit();
  if (Init && !Init->isValueDependent() && Init->isEvaluatable(AC)) {
    clang::Expr::EvalResult EVResult;
    if (Init->EvaluateAsRValue(EVResult, AC))
      if (EVResult.Val.isInt() && EVResult.Val.getInt() == 0)
        return true;
  }
  return false;
}

bool ContainerVisitor::VisitUnaryOperator(const clang::UnaryOperator *UO) {
  const clang::DeclRefExpr *DRE =
      llvm::dyn_cast_if_present<const clang::DeclRefExpr>(UO->getSubExpr());
  if (!DRE)
    return true;
  const clang::VarDecl *VD =
      llvm::dyn_cast_if_present<const clang::VarDecl>(DRE->getDecl());
  if (!VD)
    return true;
  if (!Counters.count(VD))
    return true;

  if (UO->isDecrementOp())
    return false;
  return true;
}

/* Implementation of ContainerVisitorInit */

ContainerVisitorInit::ContainerVisitorInit(clang::ASTContext &AC) : AC(AC) {}

bool ContainerVisitorInit::VisitBinaryOperator(
    const clang::BinaryOperator *BO) {
  if (BO->getOpcode() == clang::BO_Assign)
    if (BO->getRHS()->isEvaluatable(AC)) {
      clang::Expr::EvalResult EVResult;
      if (BO->getRHS()->EvaluateAsRValue(EVResult, AC))
        if (EVResult.Val.isInt() && EVResult.Val.getInt() == 0)
          return true;
    }
  return false;
}

/* Implementation of UsedContainerVisitor */

UsedContainerVisitor::UsedContainerVisitor(clang::ASTContext &AC,
                                           clang::ForStmt *ParentFS,
                                           VarDeclPtrMap &Counters)
    : AC(AC), ParentFS(ParentFS), Counters(Counters) {}

bool UsedContainerVisitor::compareToArraySize(const clang::Expr *E,
                                              const uint64_t ArraySize) {
  if (!E->isValueDependent() && E->isEvaluatable(AC)) {
    clang::Expr::EvalResult Result;
    E->EvaluateAsInt(Result, AC);
    if (*Result.Val.getInt().getRawData() != ArraySize)
      return false;
  }
  return true;
}

bool UsedContainerVisitor::VisitArraySubscriptExpr(
    clang::ArraySubscriptExpr *ASE) {
  // Extract ArraySize from Base.
  const clang::Expr *Base = ASE->getBase()->IgnoreImpCasts();
  const clang::DeclRefExpr *BaseDRE = isDeclRefExpr(Base);
  if (!BaseDRE)
    return true;
  const clang::VarDecl *BaseVD = isVarDecl(BaseDRE->getDecl());
  if (!BaseVD || !BaseVD->getType()->isConstantArrayType())
    return true;
  const clang::ConstantArrayType *CAT =
      AC.getAsConstantArrayType(BaseVD->getType());
  const uint64_t ArraySize = *CAT->getSize().getRawData();

  // Extract counter VarDecl from Idx.
  const clang::Expr *Idx = ASE->getIdx()->IgnoreImpCasts();
  const clang::DeclRefExpr *DRE = isDeclRefExpr(Idx);
  if (!DRE)
    return true;
  const clang::VarDecl *VD = isVarDecl(DRE->getDecl());
  if (!VD || !Counters.count(VD))
    return true;

  // Try to find counter variable on the LHS of condition expression.
  const clang::BinaryOperator *BOP = isBinaryOperator(ParentFS->getCond());
  if (!BOP)
    return true;
  const clang::DeclRefExpr *LHSDRE =
      isDeclRefExpr(BOP->getLHS()->IgnoreImpCasts());
  if (!LHSDRE)
    return true;
  const clang::VarDecl *LHSVD = isVarDecl(LHSDRE->getDecl());
  if (!LHSVD)
    return true;
  const clang::Expr *E = (LHSVD == VD ? BOP->getRHS()->IgnoreImpCasts()
                                      : BOP->getLHS()->IgnoreImpCasts());
  return compareToArraySize(E, ArraySize);
}

bool UsedContainerVisitor::VisitDeclRefExpr(clang::DeclRefExpr *DRE) {
  // If counter variable is used for anything other than accessing the array,
  // code is compliant with the rule.
  const clang::VarDecl *VD = isVarDecl(DRE->getDecl());
  if (!VD || !Counters.count(VD))
    return true;
  const auto &Parents = AC.getParents(*DRE);
  assert((Parents.size() > 0) && "At least one parent node expected.");
  auto Parent = Parents.begin();
  while (const clang::CastExpr *CE = Parent->get<clang::CastExpr>())
    Parent = AC.getParents(*CE).begin();
  if (VD->getType()->isPointerType()) {
    const clang::UnaryOperator *UO = Parent->get<clang::UnaryOperator>();
    if (Counters[VD].hasDerefUsageL1())
      if (IsDeref(UO))
        return false;
    if (!IsDeref(UO))
      return false;
  } else {
    if (!Parent->get<clang::ArraySubscriptExpr>())
      return false;
  }
  return true;
}

/* Implementation of ModifiedCntVisitor */

ModifiedCntVisitor::ModifiedCntVisitor(clang::ASTContext &AC,
                                       const clang::Stmt *ParentForStmt,
                                       VarDeclPtrMap &Counters,
                                       VarDeclPtrMap &ModifiedVars)
    : AC(AC), ParentForStmt(ParentForStmt), Counters(Counters),
      ModifiedVars(ModifiedVars) {}

const clang::Stmt *ModifiedCntVisitor::isForStmtParent(const clang::Stmt *S) {
  const auto &Parents = AC.getParents(*S);
  assert((Parents.size() > 0) && "At least one parent node expected.");

  // Skip ParenExpr nodes.
  if (const clang::ParenExpr *PE = Parents[0].get<clang::ParenExpr>())
    return isForStmtParent(PE);

  // Skip ImplicitCastExpr nodes.
  if (const clang::ImplicitCastExpr *ICE =
          Parents[0].get<clang::ImplicitCastExpr>())
    return isForStmtParent(ICE);

  if (Parents[0].get<clang::CallExpr>())
    return nullptr;

  if (const clang::BinaryOperator *BO = Parents[0].get<clang::BinaryOperator>())
    if (BO->getOpcode() == clang::BO_Comma)
      return isForStmtParent(BO);

  if (const clang::ForStmt *FS = Parents[0].get<clang::ForStmt>())
    if (FS == ParentForStmt)
      return FS;

  return nullptr;
}

const clang::Stmt *
ModifiedCntVisitor::isIncrementDecrementOperatorParent(const clang::Stmt *S) {
  // Get parent.
  const auto &Parents = AC.getParents(*S);
  assert((Parents.size() > 0) && "At least one parent node expected.");

  // Skip ParenExpr nodes.
  if (const clang::ParenExpr *PE = Parents[0].get<clang::ParenExpr>())
    return isIncrementDecrementOperatorParent(PE);

  // Skip ImplicitCastExpr nodes.
  if (const clang::ImplicitCastExpr *ICE =
          Parents[0].get<clang::ImplicitCastExpr>())
    return isIncrementDecrementOperatorParent(ICE);

  if (const clang::UnaryOperator *UO = Parents[0].get<clang::UnaryOperator>()) {
    if (UO->isIncrementDecrementOp())
      return UO;
    return isIncrementDecrementOperatorParent(UO);
  }

  return nullptr;
}

const clang::Stmt *
ModifiedCntVisitor::isCompoundAssignmentOpParent(const clang::Stmt *S) {
  // Get parent.
  const auto &Parents = AC.getParents(*S);
  assert((Parents.size() > 0) && "At least one parent node expected.");

  // Skip ParenExpr nodes.
  if (const clang::ParenExpr *PE = Parents[0].get<clang::ParenExpr>())
    return isCompoundAssignmentOpParent(PE);

  // Skip ImplicitCastExpr nodes.
  if (const clang::ImplicitCastExpr *ICE =
          Parents[0].get<clang::ImplicitCastExpr>())
    return isCompoundAssignmentOpParent(ICE);

  if (const clang::BinaryOperator *BO = Parents[0].get<clang::BinaryOperator>())
    if (BO->isCompoundAssignmentOp())
      if (BO->getOpcode() == clang::BO_AddAssign ||
          BO->getOpcode() == clang::BO_SubAssign)
        return BO;

  return nullptr;
}

bool ModifiedCntVisitor::VisitDeclRefExpr(clang::DeclRefExpr *DRE) {
  if (const clang::VarDecl *VD =
          llvm::dyn_cast_if_present<const clang::VarDecl>(DRE->getDecl())) {
    if (Counters.count(VD)) {

      const clang::Stmt *CAOP = isCompoundAssignmentOpParent(DRE);
      const clang::Stmt *IDOP = isIncrementDecrementOperatorParent(DRE);

      if (const clang::BinaryOperator *BO = isBinaryOperator(CAOP)) {
        clang::Expr *RHS = BO->getRHS()->IgnoreParenCasts();
        if (const clang::DeclRefExpr *RDRE = isDeclRefExpr(RHS)) {
          if (const clang::VarDecl *RVD = isVarDecl(RDRE->getDecl()))
            if (!ModifiedVars.count(RVD))
              return true;
        } else if (isIntegerLiteral(RHS)) {
          return true;
        } else if (isFloatingLiteral(RHS)) {
          return true;
        }
      }

      if (const clang::UnaryOperator *UO = isUnaryOperator(IDOP))
        if (isForStmtParent(UO))
          return true;

      return false;
    }
  }

  return true;
}

/* Implementation of CheckAssignment */

CheckAssignment::CheckAssignment(bool &C) : Correct(C) {}

bool CheckAssignment::VisitBinaryOperator(const clang::BinaryOperator *BO) {
  Correct = false;
  if (BO->getOpcode() == clang::BO_Assign)
    if (llvm::dyn_cast_if_present<clang::DeclRefExpr>(BO->getRHS()) ||
        llvm::dyn_cast_if_present<clang::ImplicitCastExpr>(BO->getRHS()) ||
        llvm::dyn_cast_if_present<clang::CharacterLiteral>(BO->getRHS()) ||
        llvm::dyn_cast_if_present<clang::IntegerLiteral>(BO->getRHS()) ||
        llvm::dyn_cast_if_present<clang::FloatingLiteral>(BO->getRHS()))
      Correct = true;
  return false;
}

/* Implementation of ForLoopVisitor */

ForLoopVisitor::ForLoopVisitor(AutocheckDiagnostic &AD, clang::ASTContext &AC)
    : AD(AD), AC(AC), WarnSingleLoopCounterFloatType(AD.IsEnabled(
                          AutocheckWarnings::singleLoopCounterFloatType)),
      WarnNonLoopCounterBool(
          AD.IsEnabled(AutocheckWarnings::nonLoopCounterBool)),
      WarnContinueStmtWellFormedLoop(
          AD.IsEnabled(AutocheckWarnings::continueStmtWellFormedLoop)),
      WarnNonLoopCounterModified(
          AD.IsEnabled(AutocheckWarnings::nonLoopCounterModified)),
      WarnLoopCounterModified(
          AD.IsEnabled(AutocheckWarnings::loopCounterModified)),
      WarnCondExprIncDecLoop(
          AD.IsEnabled(AutocheckWarnings::condExprIncDecLoop)),
      WarnInitIncPartsLoopCounter(
          AD.IsEnabled(AutocheckWarnings::initIncPartsLoopCounter)),
      WarnContainerLoopIllFormed(
          AD.IsEnabled(AutocheckWarnings::containerLoopIllFormed)),
      WarnLoopCounterIllModified(
          AD.IsEnabled(AutocheckWarnings::loopCounterIllModified)) {}

bool ForLoopVisitor::TraverseDecl(clang::Decl *D) {
  if (!D) // Can be null if code had errors.
    return true;

  clang::SourceLocation Loc = D->getBeginLoc();

  if (Loc.isInvalid() || appropriateHeaderLocation(AD, Loc)) {
    clang::RecursiveASTVisitor<ForLoopVisitor>::TraverseDecl(D);
  }
  return true;
}

void ForLoopVisitor::classifyLoopVariables(clang::ForStmt *FS) {
  VarDeclPtrMap CondVariables;
  VarDeclPtrMap IncCounterCandidates;
  // Extract variables from init part.
  VarExtractor(ControlVariables).TraverseStmt(FS->getInit());
  // Extract variables from expression part.
  ModifiedVarExtractor(&ControlVariables, IncCounterCandidates)
      .TraverseStmt(FS->getInc());
  // Extract variables from condition part.
  VarExtractor(CondVariables).TraverseStmt(FS->getCond());
  // Append control variables from condition part.
  ControlVariables.insert(CondVariables.begin(), CondVariables.end());
  // Find counters.
  findVarDeclSetIntersection(CondVariables, IncCounterCandidates,
                             CounterVariables);
  // Find non-counters.
  findVarDeclSetDifference(ControlVariables, CounterVariables,
                           NonCounterVariables);
}

// [A6-5-2] A for loop shall contain a single loop-counter which shall not have
// floating-point type.
bool ForLoopVisitor::checkNumberOfCountersAndTypes(clang::ForStmt *FS) {
  if ((CounterVariables.size() != 1) ||
      (CounterVariables.size() == 1 && (*CounterVariables.begin())
                                           .first->getType()
                                           .getTypePtr()
                                           ->isFloatingType())) {
    if (AD.IsEnabled(AutocheckWarnings::singleLoopCounterFloatType)) {
      WarnSingleLoopCounterFloatType =
          !AD.reportWarning(FS->getForLoc(),
                            AutocheckWarnings::singleLoopCounterFloatType)
               .limitReached();
    }
    return false;
  }
  return true;
}

// [M6-5-6] A loop-control-variable other than the loop-counter which is
// modified in statement shall have type bool.
bool ForLoopVisitor::checkNonCountersType(clang::ForStmt *FS) {
  VarDeclPtrMap ModifiedVariables;
  VarDeclPtrMap ModifiedNonCounters;

  // Extract all modified variables from body.
  ModifiedVarExtractor(ModifiedVariables).TraverseStmt(FS->getBody());
  // Find modified non-counter variables.
  findVarDeclSetIntersection(NonCounterVariables, ModifiedVariables,
                             ModifiedNonCounters);

  for (VarDeclPtrUsage ModifiedNonCounter : ModifiedNonCounters) {
    if (!(ModifiedNonCounter.first->getType()->isBooleanType())) {
      if (AD.IsEnabled(AutocheckWarnings::nonLoopCounterBool)) {
        WarnNonLoopCounterBool =
            !AD.reportWarning(FS->getForLoc(),
                              AutocheckWarnings::nonLoopCounterBool)
                 .limitReached();
      }
      return false;
    }
  }
  return true;
}

// [M6-6-3] The continue statement shall only be used within a well-formed for
// loop.
void ForLoopVisitor::checkIsLoopWithContinueWellFormed(clang::ForStmt *FS,
                                                       bool LS) {
  if (!LS && !ContinueStmtChecker(FS).TraverseStmt(FS->getBody()))
    WarnContinueStmtWellFormedLoop =
        !AD.reportWarning(FS->getForLoc(),
                          AutocheckWarnings::continueStmtWellFormedLoop)
             .limitReached();
}

// [M6-5-5] A loop-control-variable other than the loop-counter shall not be
// modified within condition or expression.
void ForLoopVisitor::checkIsNonCounterIllModified(clang::ForStmt *FS) {
  VarDeclPtrMap CondModifiedVars;
  VarDeclPtrMap ExprModifiedVars;

  // Extract all modified variables from condition part.
  ModifiedVarExtractor(CondModifiedVars).TraverseStmt(FS->getCond());
  // Extract all modified variables from increment part.
  ModifiedVarExtractor(ExprModifiedVars).TraverseStmt(FS->getInc());

  for (VarDeclPtrUsage NonCounterVariable : NonCounterVariables)
    if (CondModifiedVars.count(NonCounterVariable.first) ||
        ExprModifiedVars.count(NonCounterVariable.first))
      WarnNonLoopCounterModified =
          !AD.reportWarning(FS->getForLoc(),
                            AutocheckWarnings::nonLoopCounterModified)
               .limitReached();
}

// [M6-5-3] The loop-counter shall not be modified within condition or
// statement.
void ForLoopVisitor::checkIsCounterIllModified(clang::ForStmt *FS) {
  VarDeclPtrMap CondModifiedVars;
  VarDeclPtrMap StmtModifiedVars;

  // Extract all modified variables from condition part.
  ModifiedVarExtractor(CondModifiedVars).TraverseStmt(FS->getCond());
  // Extract all modified variables from body.
  ModifiedVarExtractor(StmtModifiedVars).TraverseStmt(FS->getBody());

  for (VarDeclPtrUsage CounterVariable : CounterVariables) {
    VarDeclPtrMap::iterator CMV = CondModifiedVars.find(CounterVariable.first);
    VarDeclPtrMap::iterator SMV = StmtModifiedVars.find(CounterVariable.first);
    if (
        // Check is counter modified in condition.
        (CMV != CondModifiedVars.end() &&
         CMV->second == CounterVariable.second) ||
        // Check is counter modified in statement.
        (SMV != StmtModifiedVars.end() &&
         SMV->second == CounterVariable.second) ||
        // Check is counter indirectly modified in statement.
        (SMV != StmtModifiedVars.end() && !SMV->second.hasDerefUsage() &&
         (CounterVariable.second.hasDerefUsageL1() ||
          CounterVariable.second.hasDerefUsageL2())))
      WarnLoopCounterModified =
          !AD.reportWarning(FS->getForLoc(),
                            AutocheckWarnings::loopCounterModified)
               .limitReached();
  }
}

// [M6-5-2] If loop-counter is not modified by -- or ++, then, within condition,
// the loop-counter shall only be used as an operand to <=, <, > or >=.
void ForLoopVisitor::checkIsCounterIncorrectlyUsed(clang::ForStmt *FS) {
  VarDeclPtrMap IncDecModifiedVariables;
  VarDeclPtrMap NonIncDecModifiedCounters;

  IncDecModifiedVars(IncDecModifiedVariables).TraverseStmt(FS->getInit());
  IncDecModifiedVars(IncDecModifiedVariables).TraverseStmt(FS->getCond());
  IncDecModifiedVars(IncDecModifiedVariables).TraverseStmt(FS->getInc());

  VarDeclPtrMap ModVars;
  ModifiedVarExtractor(ModVars).TraverseStmt(FS->getInit());
  ModifiedVarExtractor(ModVars).TraverseStmt(FS->getCond());
  ModifiedVarExtractor(ModVars).TraverseStmt(FS->getInc());

  for (auto &var : IncDecModifiedVariables) {
    const clang::VarDecl *VD = var.first;
    if (ModVars.count(VD))
      IncDecModifiedVariables[VD] = ModVars[VD];
  }

  findVarDeclSetDifference(CounterVariables, IncDecModifiedVariables,
                           NonIncDecModifiedCounters);

  if (NonIncDecModifiedCounters.size()) {
    VarDeclPtrMap RelOpComparedVariables;
    VarDeclPtrMap NonRelOpComparedCounters;

    RelOpVarExtractor(RelOpComparedVariables).TraverseStmt(FS->getCond());

    findVarDeclSetDifference(CounterVariables, RelOpComparedVariables,
                             NonRelOpComparedCounters);

    for (VarDeclPtrUsage NonIncDecModifiedCounter : NonIncDecModifiedCounters) {
      VarDeclPtrMap::iterator NROCC =
          NonRelOpComparedCounters.find(NonIncDecModifiedCounter.first);
      if ((NROCC != NonRelOpComparedCounters.end()) &&
          (NROCC->second == NonIncDecModifiedCounter.second))
        WarnCondExprIncDecLoop =
            !AD.reportWarning(FS->getForLoc(),
                              AutocheckWarnings::condExprIncDecLoop)
                 .limitReached();
    }
  }
}

// [A6-5-4] For-init-statement and expression should not perform actions other
// than loop-counter initialization and modification.
void ForLoopVisitor::checkLoopActions(clang::ForStmt *FS) {
  VarDeclPtrMap InitVariables;
  VarDeclPtrMap ExpressionVariables;

  if (ForbiddenActVisitor().TraverseStmt(FS->getInit())) {
    VarExtractor(InitVariables).TraverseStmt(FS->getInit());
    VarExtractor(ExpressionVariables).TraverseStmt(FS->getInc());

    for (VarDeclPtrUsage NCVar : NonCounterVariables) {
      // If InitVariables contain a non counter variable, it is okay only if
      // there is only one such variable and it is used for initialization.
      bool InitOK = !InitVariables.count(NCVar.first);
      if (!InitOK && (InitVariables.size() == 2))
        CheckAssignment(InitOK).TraverseStmt(FS->getInit());
      bool ExprOK = !ExpressionVariables.count(NCVar.first);
      if (!InitOK || !ExprOK) {
        WarnInitIncPartsLoopCounter =
            !AD.reportWarning(!InitOK ? FS->getInit()->getBeginLoc()
                                      : FS->getInc()->getBeginLoc(),
                              AutocheckWarnings::initIncPartsLoopCounter)
                 .limitReached();
        return;
      }
    }

    // If InitVariables contains only one variable it is okay only if it is
    // assigned by a literal.
    if (InitVariables.size() == 1) {
      bool InitOK = true;
      if (llvm::dyn_cast_if_present<clang::BinaryOperator>(FS->getInit())) {
        CheckAssignment(InitOK).TraverseStmt(FS->getInit());
      }
      if (!InitOK) {
        WarnInitIncPartsLoopCounter =
            !AD.reportWarning(FS->getInit()->getBeginLoc(),
                              AutocheckWarnings::initIncPartsLoopCounter)
                 .limitReached();
        return;
      }
    }
    return;
  }
  AD.reportWarning(FS->getForLoc(), AutocheckWarnings::initIncPartsLoopCounter);
}

// [A6-5-1] A for-loop that loops through all elements of the container and does
// not use its loop-counter shall not be used.
void ForLoopVisitor::checkContainerLoop(clang::ForStmt *FS) {
  VarDeclPtrMap UsedVariables;
  VarDeclPtrMap UsedCounters;
  VarDeclPtrMap UnusedCounters;
  llvm::SmallVector<const clang::VarDecl *, 0> UnusedVariablesErase;

  VarExtractor(UsedVariables).TraverseStmt(FS->getBody());
  findVarDeclSetIntersection(CounterVariables, UsedVariables, UsedCounters);
  findVarDeclSetDifference(CounterVariables, UsedCounters, UnusedCounters);

  for (VarDeclPtrUsage UnusedVariable : UnusedCounters)
    if (UsedVariables.count(UnusedVariable.first))
      if (UsedVariables[UnusedVariable.first].hasDerefUsage() &&
          !UnusedVariable.second.hasDerefUsage())
        UnusedVariablesErase.push_back(UnusedVariable.first);

  for (const clang::VarDecl *UnusedVariable : UnusedVariablesErase)
    UnusedCounters.erase(UnusedVariable);

  if (!UnusedCounters.empty()) {
    ContainerVisitor CV(AC, FS, UnusedCounters);
    ContainerVisitorInit CVI(AC);

    if (CVI.TraverseStmt(FS->getInit()) && CV.TraverseStmt(FS->getInit()) &&
        CV.TraverseStmt(FS->getCond()) && CV.TraverseStmt(FS->getInc())) {
      WarnContainerLoopIllFormed =
          !AD.reportWarning(FS->getForLoc(),
                            AutocheckWarnings::containerLoopIllFormed)
               .limitReached();
      return;
    }
  }

  if (!UsedCounters.empty()) {
    UsedContainerVisitor UCV(AC, FS, UsedCounters);
    if (UCV.TraverseStmt(FS->getBody()))
      WarnContainerLoopIllFormed =
          !AD.reportWarning(FS->getForLoc(),
                            AutocheckWarnings::containerLoopIllFormed)
               .limitReached();
  }
}

// [M6-5-4] The loop-counter shall be modified by one of: --, ++, -= n, or += n;
// where n remains constant for the duration of the loop.
void ForLoopVisitor::isCounterCorrectlyModified(clang::ForStmt *FS) {
  VarDeclPtrMap ModifiedVars;

  // Extract all modified variables from condition part.
  ModifiedVarExtractor(ModifiedVars).TraverseStmt(FS->getCond());
  // Extract all modified variables from loop's body.
  ModifiedVarExtractor(ModifiedVars).TraverseStmt(FS->getBody());
  // Extract all modified variables from expression part.
  ModifiedVarExtractor(ModifiedVars).TraverseStmt(FS->getInc());

  ModifiedCntVisitor MCV(AC, FS, CounterVariables, ModifiedVars);
  if (!MCV.TraverseStmt(FS->getInc()))
    WarnLoopCounterIllModified =
        !AD.reportWarning(FS->getForLoc(),
                          AutocheckWarnings::loopCounterIllModified)
             .limitReached();
}

bool ForLoopVisitor::VisitForStmt(clang::ForStmt *FS) {
  ControlVariables.clear();
  CounterVariables.clear();
  NonCounterVariables.clear();

  classifyLoopVariables(FS);
  bool LoopStatus1 = false;
  bool LoopStatus2 = false;

  if (WarnSingleLoopCounterFloatType || WarnContinueStmtWellFormedLoop)
    LoopStatus1 = checkNumberOfCountersAndTypes(FS);

  if (WarnNonLoopCounterBool || WarnContinueStmtWellFormedLoop)
    LoopStatus2 = checkNonCountersType(FS);

  if (WarnContinueStmtWellFormedLoop)
    checkIsLoopWithContinueWellFormed(FS, LoopStatus1 && LoopStatus2);

  if (WarnNonLoopCounterModified)
    checkIsNonCounterIllModified(FS);

  if (WarnLoopCounterModified)
    checkIsCounterIllModified(FS);

  if (WarnCondExprIncDecLoop)
    checkIsCounterIncorrectlyUsed(FS);

  if (WarnInitIncPartsLoopCounter)
    checkLoopActions(FS);

  if (WarnContainerLoopIllFormed)
    checkContainerLoop(FS);

  if (WarnLoopCounterIllModified)
    isCounterCorrectlyModified(FS);

  return WarnSingleLoopCounterFloatType || WarnNonLoopCounterBool ||
         WarnContinueStmtWellFormedLoop || WarnNonLoopCounterModified ||
         WarnLoopCounterModified || WarnCondExprIncDecLoop ||
         WarnInitIncPartsLoopCounter || WarnContainerLoopIllFormed ||
         WarnLoopCounterIllModified;
}

void ForLoopVisitor::run(clang::TranslationUnitDecl *TUD) {
  if (WarnSingleLoopCounterFloatType || WarnNonLoopCounterBool ||
      WarnContinueStmtWellFormedLoop || WarnNonLoopCounterModified ||
      WarnLoopCounterModified || WarnCondExprIncDecLoop ||
      WarnInitIncPartsLoopCounter || WarnContainerLoopIllFormed ||
      WarnLoopCounterIllModified)
    TraverseDecl(TUD);
}

} // namespace autocheck
