//===--- ExpressionsVisitor.cpp - Visitor for expression related checks ---===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file implements the ExpressionsVisitor class and all classes derived
// from ExpressionsVisitorInterface.
//
//===----------------------------------------------------------------------===//

#include "AST/ExpressionsVisitor.h"

#include "AST/SideEffectChecker.h"
#include "Diagnostics/AutocheckDiagnostic.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/Basic/SourceManager.h"

namespace autocheck {

ExpressionsVisitorInterface::~ExpressionsVisitorInterface() {}

using EVI = ExpressionsVisitorInterface;
bool EVI::PreTraverseStmt(clang::Stmt *) { return true; }
bool EVI::PostTraverseStmt(clang::Stmt *) { return true; }
bool EVI::VisitLambdaExpr(const clang::LambdaExpr *) { return true; }
bool EVI::VisitUnaryOperator(const clang::UnaryOperator *) { return true; }
bool EVI::VisitConditionalOperator(const clang::ConditionalOperator *) {
  return true;
}
bool EVI::VisitBinaryOperator(const clang::BinaryOperator *) { return true; }
bool EVI::VisitReturnStmt(const clang::ReturnStmt *) { return true; }
bool EVI::VisitCallExpr(const clang::CallExpr *) { return true; }
bool EVI::VisitUnaryExprOrTypeTraitExpr(
    const clang::UnaryExprOrTypeTraitExpr *) {
  return true;
}
bool EVI::VisitCXXThrowExpr(const clang::CXXThrowExpr *) { return true; }

/* Implementation of ImplicitlyCapturedVarVisitor */

ImplicitlyCapturedVarVisitor::ImplicitlyCapturedVarVisitor(
    AutocheckDiagnostic &AD)
    : AD(AD) {}

bool ImplicitlyCapturedVarVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::implicitlyCapturedVar);
}

bool ImplicitlyCapturedVarVisitor::VisitLambdaExpr(
    const clang::LambdaExpr *LE) {
  for (const auto &LC : LE->implicit_captures()) {
    if (!LC.capturesVariable())
      continue;
    const clang::ValueDecl *ValDecl = LC.getCapturedVar();
    if (const clang::VarDecl *VD =
            llvm::dyn_cast_if_present<clang::VarDecl>(ValDecl)) {
      // It is allowed to implicitly capture variables with non-automatic
      // storage duration.
      if (VD->getStorageDuration() == clang::StorageDuration::SD_Automatic) {
        if (AD.reportWarning(LE->getBeginLoc(),
                             AutocheckWarnings::implicitlyCapturedVar)
                .limitReached())
          return false;
      }
    }
  }

  return true;
}

/* Implementation of IncDecOpMixedVisitor */

IncDecOpMixedVisitor::IncDecOpMixedVisitor(AutocheckDiagnostic &AD,
                                           clang::ASTContext &ASTCtx)
    : AD(AD), ASTCtx(ASTCtx) {}

bool IncDecOpMixedVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::incDecOpMixed);
}

bool IncDecOpMixedVisitor::VisitUnaryOperator(const clang::UnaryOperator *UO) {
  if (UO->isIncrementDecrementOp() && getIncrementDecrementOpParent(UO)) {
    return !AD.reportWarning(UO->getExprLoc(), AutocheckWarnings::incDecOpMixed)
                .limitReached();
  }
  return true;
}

bool IncDecOpMixedVisitor::getIncrementDecrementOpParent(
    const clang::Expr *E) const {
  const auto &Parents = ASTCtx.getParents(*E);
  assert((Parents.size() > 0) && "At least one parent node expected.");

  if (Parents[0].get<clang::BinaryOperator>() ||
      Parents[0].get<clang::UnaryOperator>() ||
      Parents[0].get<clang::ConditionalOperator>() ||
      Parents[0].get<clang::ImplicitCastExpr>() ||
      Parents[0].get<clang::ParenExpr>() ||
      Parents[0].get<clang::ArraySubscriptExpr>())
    return true;

  return false;
}

/* Implementation of TernaryOpSubExprVisitor */

TernaryOpSubExprVisitor::TernaryOpSubExprVisitor(AutocheckDiagnostic &AD,
                                                 clang::ASTContext &ASTCtx)
    : AD(AD), ASTCtx(ASTCtx) {}

bool TernaryOpSubExprVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::ternaryOpSubExpr);
}

bool TernaryOpSubExprVisitor::VisitConditionalOperator(
    const clang::ConditionalOperator *CO) {
  if (getTernaryOpParent(CO)) {
    return !AD.reportWarning(CO->getExprLoc(),
                             AutocheckWarnings::ternaryOpSubExpr)
                .limitReached();
  }
  return true;
}

const clang::Stmt *
TernaryOpSubExprVisitor::getTernaryOpParent(const clang::Stmt *S) const {
  const auto &Parents = ASTCtx.getParents(*S);
  assert((Parents.size() > 0) && "At least one parent node expected.");

  // Implicit cast expressions can be omitted.
  if (const clang::ImplicitCastExpr *ICE =
          Parents[0].get<clang::ImplicitCastExpr>())
    return getTernaryOpParent(ICE);

  // Permissible uses of a conditional expression:
  if (Parents[0].get<clang::ReturnStmt>() ||
      Parents[0].get<clang::CallExpr>() ||
      Parents[0].get<clang::InitListExpr>())
    return nullptr;

  if (const clang::BinaryOperator *BO = Parents[0].get<clang::BinaryOperator>())
    if (BO->isAssignmentOp())
      return nullptr;

  if (const clang::VarDecl *VD = Parents[0].get<clang::VarDecl>()) {
    const auto &VDParents = ASTCtx.getParents(*VD);
    assert((Parents.size() > 0) && "At least one parent node expected.");
    if (VDParents[0].get<clang::DeclStmt>())
      return nullptr;
  }

  return S;
}

/* Implementation of AssignmentOpSubExprVisitor */

AssignmentOpSubExprVisitor::AssignmentOpSubExprVisitor(AutocheckDiagnostic &AD,
                                                       clang::ASTContext &AC)
    : AD(AD), AC(AC) {}

bool AssignmentOpSubExprVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::assignmentOpSubExpr);
}

bool AssignmentOpSubExprVisitor::VisitBinaryOperator(
    const clang::BinaryOperator *BO) {
  if ((BO->isAssignmentOp() || BO->isCompoundAssignmentOp()) &&
      getAssignmentOpParent(BO)) {
    return !AD.reportWarning(BO->getExprLoc(),
                             AutocheckWarnings::assignmentOpSubExpr)
                .limitReached();
  }
  return true;
}

bool AssignmentOpSubExprVisitor::getAssignmentOpParent(
    const clang::Expr *E) const {
  const auto &Parents = AC.getParents(*E);
  assert((Parents.size() > 0) && "At least one parent node expected.");

  if (!(Parents[0].get<clang::Expr>()))
    return false;

  return true;
}

/* Implementation of NestedLambdaVisitor */

NestedLambdaVisitor::NestedLambdaVisitor(AutocheckDiagnostic &AD)
    : AD(AD), LambdaCounter(0) {}

bool NestedLambdaVisitor::PreTraverseStmt(clang::Stmt *S) {
  if (llvm::dyn_cast_if_present<clang::LambdaExpr>(S)) {
    if (LambdaCounter > 0) {
      if (AD.reportWarning(S->getBeginLoc(), AutocheckWarnings::nestedLambda)
              .limitReached())
        return false;
    }

    ++LambdaCounter;
  }

  return true;
}

bool NestedLambdaVisitor::PostTraverseStmt(clang::Stmt *S) {
  if (llvm::dyn_cast_if_present<clang::LambdaExpr>(S))
    --LambdaCounter;

  return true;
}

bool NestedLambdaVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::nestedLambda);
}

/* Implementation of LambdaReturnTypeVisitor */

LambdaReturnTypeVisitor::LambdaReturnTypeVisitor(AutocheckDiagnostic &AD)
    : AD(AD) {}

bool LambdaReturnTypeVisitor::PreTraverseStmt(clang::Stmt *S) {
  if (llvm::dyn_cast_if_present<clang::LambdaExpr>(S)) {
    ReturnStmtSeenVector.push_back(false);
  }

  return true;
}

bool LambdaReturnTypeVisitor::PostTraverseStmt(clang::Stmt *S) {
  if (const clang::LambdaExpr *LE =
          llvm::dyn_cast_if_present<const clang::LambdaExpr>(S)) {
    if (!LE->hasExplicitResultType() && ReturnStmtSeenVector.back()) {
      if (AD.reportWarning(LE->getBeginLoc(),
                           AutocheckWarnings::lambdaReturnTypeUnspecified)
              .limitReached())
        return false;
    }
    ReturnStmtSeenVector.pop_back();
  }

  return true;
}

bool LambdaReturnTypeVisitor::VisitReturnStmt(const clang::ReturnStmt *RS) {
  if (RS->getRetValue() && !ReturnStmtSeenVector.empty())
    ReturnStmtSeenVector.back() = true;

  return true;
}

bool LambdaReturnTypeVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::lambdaReturnTypeUnspecified);
}

/* Implementation of StdMoveUsedConstVisitor */

StdMoveUsedConstVisitor::StdMoveUsedConstVisitor(AutocheckDiagnostic &AD)
    : AD(AD) {}

bool StdMoveUsedConstVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::stdMoveUsedConst);
}

bool StdMoveUsedConstVisitor::VisitCallExpr(const clang::CallExpr *CE) {
  if (CE->isCallToStdMove()) {
    const auto *ExprArg = CE->getArg(0);
    const clang::QualType Type = ExprArg->getType();
    if (Type.isConstQualified() && Type.getTypePtr()->isObjectType()) {
      return !AD.reportWarning(ExprArg->getBeginLoc(),
                               AutocheckWarnings::stdMoveUsedConst)
                  .limitReached();
    }
  }
  return true;
}

/* Implementation of CharacterHandlingFunctionsVisitor */

CharacterHandlingFunctionsVisitor::CharacterHandlingFunctionsVisitor(
    AutocheckDiagnostic &AD)
    : AD(AD), SM(AD.GetSourceManager()) {}

bool CharacterHandlingFunctionsVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::characterHandlingFunctions);
}

bool CharacterHandlingFunctionsVisitor::VisitCallExpr(
    const clang::CallExpr *CE) {
  // Obtaining DeclRefExpr and then FunctionDecl of called function from
  // CallExpr, in order to get location of the declaration.
  const clang::Expr *Callee = CE->getCallee();
  const clang::DeclRefExpr *DRF =
      llvm::dyn_cast_if_present<clang::DeclRefExpr>(Callee->IgnoreImpCasts());
  if (!DRF)
    return true;
  const clang::FunctionDecl *FD =
      llvm::dyn_cast_if_present<clang::FunctionDecl>(DRF->getDecl());
  if (!FD)
    return true;

  clang::PresumedLoc DeclLocation =
      SM.getPresumedLoc(FD->getFirstDecl()->getBeginLoc());

  if (DeclLocation.isInvalid() ||
      !SM.isInSystemHeader(FD->getFirstDecl()->getBeginLoc()))
    return true;

  llvm::StringRef FileName{DeclLocation.getFilename()};

  // Stop analyzing current CallExpr if it's declaration isn't inside ctype
  // header.
  if (!FileName.ends_with("ctype.h") && !FileName.ends_with("cctype"))
    return true;

  // Check if argument is already unsigned char.
  const clang::Expr *Arg = CE->getArg(0);
  if (const clang::DeclRefExpr *DRF =
          llvm::dyn_cast_if_present<clang::DeclRefExpr>(
              Arg->IgnoreImpCasts())) {
    if (const clang::ValueDecl *VD = DRF->getDecl()) {
      const clang::Type *ArgumentType = VD->getType().getTypePtr();
      if (ArgumentType->isUnsignedIntegerType() && ArgumentType->isCharType())
        return true;
    }
  }

  // Check if argument in CallExpr is explicitly casted to unsigned char type,
  // if not, report warning.
  const clang::ExplicitCastExpr *ECE =
      llvm::dyn_cast_if_present<clang::ExplicitCastExpr>(Arg->IgnoreImpCasts());

  if (!ECE) {
    return !AD.reportWarning(Arg->getBeginLoc(),
                             AutocheckWarnings::characterHandlingFunctions)
                .limitReached();
  }

  const clang::Type *CastType =
      ECE->getTypeInfoAsWritten()->getType().getTypePtr();

  if (CastType->isUnsignedIntegerType() && CastType->isCharType())
    return true;

  return !AD.reportWarning(ECE->getBeginLoc(),
                           AutocheckWarnings::characterHandlingFunctions)
              .limitReached();
}

/* Implementation of StdFunctionUsed */

bool cstdlibMemfuncDiagnostics(AutocheckDiagnostic &AD,
                               const std::string &HeaderName,
                               clang::SourceLocation &SL,
                               const std::string &FuncName) {
  if (HeaderName != "cstdlib" && HeaderName != "stdlib.h")
    return true;
  if (FuncName == "malloc" || FuncName == "calloc" || FuncName == "realloc" ||
      FuncName == "free") {
    return !AD.reportWarning(SL, AutocheckWarnings::cstdlibMemfuncUsed)
                .limitReached();
  }
  return true;
}

bool cstdlibFuncDiagnostics(AutocheckDiagnostic &AD,
                            const std::string &HeaderName,
                            clang::SourceLocation &SL,
                            const std::string &FuncName) {
  if (HeaderName != "cstdlib" && HeaderName != "stdlib.h")
    return true;
  if (FuncName == "abort" || FuncName == "exit" || FuncName == "getenv" ||
      FuncName == "system") {
    return !AD.reportWarning(SL, AutocheckWarnings::cstdlibFuncUsed)
                .limitReached();
  }
  return true;
}

bool cstringFuncDiagnostics(AutocheckDiagnostic &AD,
                            const std::string &HeaderName,
                            clang::SourceLocation &SL,
                            const std::string &FuncName) {
  if (HeaderName != "cstring" && HeaderName != "string.h")
    return true;
  if (FuncName == "strcpy" || FuncName == "strcmp" || FuncName == "strcat" ||
      FuncName == "strchr" || FuncName == "strspn" || FuncName == "strcspn" ||
      FuncName == "strpbrk" || FuncName == "strrchr" || FuncName == "strstr" ||
      FuncName == "strtok" || FuncName == "strlen") {
    return !AD.reportWarning(SL, AutocheckWarnings::cstringFuncUsed)
                .limitReached();
  }
  return true;
}

bool setjmpLongjmpFuncDiagnostics(AutocheckDiagnostic &AD,
                                  const std::string &HeaderName,
                                  clang::SourceLocation &SL,
                                  const std::string &FuncName) {
  if (HeaderName != "csetjmp" && HeaderName != "setjmp.h")
    return true;
  if (FuncName == "longjmp") {
    return !AD.reportWarning(SL, AutocheckWarnings::setjmpLongjmpUsed)
                .limitReached();
  }
  return true;
}

bool bindFuncDiagnostics(AutocheckDiagnostic &AD, const std::string &HeaderName,
                         clang::SourceLocation &SL,
                         const std::string &FuncName) {
  if (HeaderName != "functional")
    return true;
  if (FuncName == "bind") {
    return !AD.reportWarning(SL, AutocheckWarnings::stdBindUsed).limitReached();
  }
  return true;
}

bool randFuncDiagnostics(AutocheckDiagnostic &AD, const std::string &HeaderName,
                         clang::SourceLocation &SL,
                         const std::string &FuncName) {
  if (HeaderName != "cstdlib" && HeaderName != "stdlib.h")
    return true;
  if (FuncName == "rand") {
    return !AD.reportWarning(SL, AutocheckWarnings::stdRandUsed).limitReached();
  }
  return true;
}

bool randomShuffleFuncDiagnostics(AutocheckDiagnostic &AD,
                                  const std::string &HeaderName,
                                  clang::SourceLocation &SL,
                                  const std::string &FuncName) {
  if (HeaderName != "cstl_algo" && HeaderName != "stl_algo.h" &&
      HeaderName != "algorithm")
    return true;
  if (FuncName == "random_shuffle") {
    return !AD.reportWarning(SL, AutocheckWarnings::stdRandUsed).limitReached();
  }
  return true;
}

StdFunctionUsed::StdFunctionUsed(AutocheckDiagnostic &AD) : AD(AD) {
  const AutocheckContext &Context = AD.GetContext();
  if (Context.isEnabled(AutocheckWarnings::cstdlibMemfuncUsed))
    AllFunctions.push_front(&cstdlibMemfuncDiagnostics);
  if (Context.isEnabled(AutocheckWarnings::cstdlibFuncUsed))
    AllFunctions.push_front(&cstdlibFuncDiagnostics);
  if (Context.isEnabled(AutocheckWarnings::cstringFuncUsed))
    AllFunctions.push_front(&cstringFuncDiagnostics);
  if (Context.isEnabled(AutocheckWarnings::setjmpLongjmpUsed))
    AllFunctions.push_front(&setjmpLongjmpFuncDiagnostics);
  if (Context.isEnabled(AutocheckWarnings::stdBindUsed))
    AllFunctions.push_front(&bindFuncDiagnostics);
  if (Context.isEnabled(AutocheckWarnings::stdRandUsed)) {
    AllFunctions.push_front(&randFuncDiagnostics);
    AllFunctions.push_front(&randomShuffleFuncDiagnostics);
  }
}

bool StdFunctionUsed::isFlagPresent(const AutocheckContext &Context) {
  return (Context.isEnabled(AutocheckWarnings::cstdlibMemfuncUsed) ||
          Context.isEnabled(AutocheckWarnings::cstdlibFuncUsed) ||
          Context.isEnabled(AutocheckWarnings::cstringFuncUsed) ||
          Context.isEnabled(AutocheckWarnings::setjmpLongjmpUsed) ||
          Context.isEnabled(AutocheckWarnings::stdBindUsed) ||
          Context.isEnabled(AutocheckWarnings::stdRandUsed));
}

bool StdFunctionUsed::VisitCallExpr(const clang::CallExpr *CE) {
  clang::SourceLocation SL = CE->getBeginLoc();
  if (const clang::FunctionDecl *FD = CE->getDirectCallee()) {
    clang::SourceLocation SLDecl = FD->getNameInfo().getLoc();
    if (!AD.GetSourceManager().isInSystemHeader(SLDecl))
      return true;
    const std::string FullHeaderName =
        AD.GetSourceManager().getFilename(SLDecl).str();
    const std::string::size_type Pos = FullHeaderName.find_last_of("/\\");
    const std::string HeaderName = FullHeaderName.substr(Pos + 1);
    const std::string FuncName = FD->getNameInfo().getAsString();

    // After limit for some AutocheckWarning is reached, remove corresponding
    // function from list. When list is empty, we stop the visitor.
    AllFunctions.remove_if(
        [this, &HeaderName, &FuncName, &SL](bool (*limitReached)(
            AutocheckDiagnostic &, const std::string &, clang::SourceLocation &,
            const std::string &)) {
          return !(*limitReached)(AD, HeaderName, SL, FuncName);
        });
    if (AllFunctions.empty())
      return false;
  }
  return true;
}

/* Implementation of ImplicitBitwiseBinOpConversionVisitor */

ImplicitBitwiseBinOpConversionVisitor::ImplicitBitwiseBinOpConversionVisitor(
    AutocheckDiagnostic &AD, clang::ASTContext &ASTCtx)
    : AD(AD), ASTCtx(ASTCtx) {
  AllowedBitwiseBinaryOps.insert(clang::BO_And);
  AllowedBitwiseBinaryOps.insert(clang::BO_Or);
  AllowedBitwiseBinaryOps.insert(clang::BO_Shl);
  AllowedBitwiseBinaryOps.insert(clang::BO_Shr);
  AllowedBitwiseBinaryOps.insert(clang::BO_Xor);

  AllowedBitwiseBinaryOps.insert(clang::BO_AndAssign);
  AllowedBitwiseBinaryOps.insert(clang::BO_OrAssign);
  AllowedBitwiseBinaryOps.insert(clang::BO_ShlAssign);
  AllowedBitwiseBinaryOps.insert(clang::BO_ShrAssign);
  AllowedBitwiseBinaryOps.insert(clang::BO_XorAssign);
}

bool ImplicitBitwiseBinOpConversionVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::implicitBitwiseBinopConversion);
}

bool ImplicitBitwiseBinOpConversionVisitor::VisitBinaryOperator(
    const clang::BinaryOperator *BO) {
  const clang::BinaryOperator::Opcode BinOpCode = BO->getOpcode();
  // Check if this is a bitwise binary operator.
  if (AllowedBitwiseBinaryOps.contains(BinOpCode)) {
    const clang::Expr *LHS = BO->getLHS()->IgnoreImpCasts();
    const clang::Expr *RHS = BO->getRHS()->IgnoreImpCasts();
    // Check if any operand is a constant.
    if ((!LHS->isValueDependent()) && (!RHS->isValueDependent()) &&
        (!(LHS->isIntegerConstantExpr(ASTCtx) ||
           RHS->isIntegerConstantExpr(ASTCtx)))) {
      // Check if types are the same.
      if (LHS->getType().getDesugaredType(ASTCtx) !=
          RHS->getType().getDesugaredType(ASTCtx)) {
        return !AD.reportWarning(
                      BO->getOperatorLoc(),
                      AutocheckWarnings::implicitBitwiseBinopConversion,
                      BO->getSourceRange())
                    .limitReached();
      }
    }
  }
  return true;
}

/* Implementation of SizeofSideEffectVisitor */

SizeofSideEffectVisitor::SizeofSideEffectVisitor(AutocheckDiagnostic &AD,
                                                 clang::ASTContext &AC)
    : AD(AD), AC(AC) {}

bool SizeofSideEffectVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::sizeofSideEffect);
}

bool SizeofSideEffectVisitor::VisitUnaryExprOrTypeTraitExpr(
    const clang::UnaryExprOrTypeTraitExpr *UOTTE) {
  // Check if the operator is sizeof.
  if (UOTTE->getKind() == clang::UETT_SizeOf) {
    // Check if the argument is an expression with side effects.
    if (!UOTTE->isArgumentType() &&
        UOTTE->getArgumentExpr()->IgnoreParenImpCasts()->HasSideEffects(AC)) {
      return !AD.reportWarning(UOTTE->getOperatorLoc(),
                               AutocheckWarnings::sizeofSideEffect)
                  .limitReached();
    }
  }
  return true;
}

/* Implementation of ConditionalOpVisitor */

ConditionalOpVisitor::ConditionalOpVisitor(AutocheckDiagnostic &AD) : AD(AD) {}

bool ConditionalOpVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::conditionalOpOperandType);
}

bool ConditionalOpVisitor::VisitConditionalOperator(
    const clang::ConditionalOperator *CO) {
  if (const clang::ImplicitCastExpr *ICE =
          llvm::dyn_cast_if_present<clang::ImplicitCastExpr>(CO->getCond())) {
    // TODO: Check CastKind
    if (!ICE->getSubExpr()->getType()->isBooleanType() &&
        ICE->getType()->isBooleanType()) {
      return !AD.reportWarning(CO->getCond()->getBeginLoc(),
                               AutocheckWarnings::conditionalOpOperandType)
                  .limitReached();
    }
  }

  return true;
}

/* Implementation of LogicalOpBoolOperandVisitor */

LogicalOpBoolOperandVisitor::LogicalOpBoolOperandVisitor(
    AutocheckDiagnostic &AD)
    : AD(AD) {}

bool LogicalOpBoolOperandVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::notAndOrOpsBoolOperands);
}

bool LogicalOpBoolOperandVisitor::VisitUnaryOperator(
    const clang::UnaryOperator *UO) {
  if (UO->getOpcode() == clang::UO_LNot) {
    if (const clang::ImplicitCastExpr *ICE =
            llvm::dyn_cast_if_present<clang::ImplicitCastExpr>(
                UO->getSubExpr())) {
      // TODO: Check CastKind
      if (!ICE->getSubExpr()->getType()->isBooleanType()) {
        return !AD.reportWarning(UO->getSubExpr()->getBeginLoc(),
                                 AutocheckWarnings::notAndOrOpsBoolOperands)
                    .limitReached();
      }
    }
  }
  return true;
}

bool LogicalOpBoolOperandVisitor::VisitBinaryOperator(
    const clang::BinaryOperator *BO) {
  if (BO->getOpcode() == clang::BO_LAnd || BO->getOpcode() == clang::BO_LOr) {
    // Check left operand.
    if (const clang::ImplicitCastExpr *ICE =
            llvm::dyn_cast_if_present<clang::ImplicitCastExpr>(BO->getLHS())) {
      // TODO: Check CastKind
      if (!ICE->getSubExpr()->getType()->isBooleanType()) {
        return !AD.reportWarning(BO->getLHS()->getBeginLoc(),
                                 AutocheckWarnings::notAndOrOpsBoolOperands)
                    .limitReached();
      }
    }

    // Check right operand.
    if (const clang::ImplicitCastExpr *ICE =
            llvm::dyn_cast_if_present<clang::ImplicitCastExpr>(BO->getRHS())) {
      // TODO: Check CastKind
      if (!ICE->getSubExpr()->getType()->isBooleanType()) {
        return !AD.reportWarning(BO->getRHS()->getBeginLoc(),
                                 AutocheckWarnings::notAndOrOpsBoolOperands)
                    .limitReached();
      }
    }
  }
  return true;
}

/* Implementation of UnaryMinusOnUnsignedVisitor */

UnaryMinusOnUnsignedVisitor::UnaryMinusOnUnsignedVisitor(
    AutocheckDiagnostic &AD)
    : AD(AD) {}

bool UnaryMinusOnUnsignedVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::unaryMinusOnUnsignedType);
}

bool UnaryMinusOnUnsignedVisitor::VisitUnaryOperator(
    const clang::UnaryOperator *UO) {
  if (UO->getOpcode() == clang::UO_Minus) {
    if (UO->getSubExpr()->getType()->isUnsignedIntegerOrEnumerationType()) {
      return !AD.reportWarning(UO->getSubExpr()->getBeginLoc(),
                               AutocheckWarnings::unaryMinusOnUnsignedType)
                  .limitReached();
    }
  }

  return true;
}

/* Implementation of ExceptionObjectPtrVisitor */

ExceptionObjectPtrVisitor::ExceptionObjectPtrVisitor(AutocheckDiagnostic &AD)
    : AD(AD) {}

bool ExceptionObjectPtrVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::exceptionObjectIsPointer);
}

bool ExceptionObjectPtrVisitor::VisitCXXThrowExpr(
    const clang::CXXThrowExpr *TE) {
  if (TE->getSubExpr() && TE->getSubExpr()->getType()->isPointerType()) {
    return !AD.reportWarning(TE->getSubExpr()->getBeginLoc(),
                             AutocheckWarnings::exceptionObjectIsPointer)
                .limitReached();
  }

  return true;
}

/* Implementation of ExceptionObjectNullVisitor */

ExceptionObjectNullVisitor::ExceptionObjectNullVisitor(AutocheckDiagnostic &AD)
    : AD(AD) {}

bool ExceptionObjectNullVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::nullThrownExplicitly);
}

bool ExceptionObjectNullVisitor::VisitCXXThrowExpr(
    const clang::CXXThrowExpr *TE) {
  if (llvm::dyn_cast_if_present<clang::GNUNullExpr>(TE->getSubExpr())) {
    return !AD.reportWarning(TE->getSubExpr()->getBeginLoc(),
                             AutocheckWarnings::nullThrownExplicitly)
                .limitReached();
  }

  return true;
}

/* Implementation of BitwiseUnsignedOperandsVisitor */

BitwiseUnsignedOperandsVisitor::BitwiseUnsignedOperandsVisitor(
    AutocheckDiagnostic &AD)
    : AD(AD) {}

bool BitwiseUnsignedOperandsVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::bitwiseOperandNotUnsigned);
}

bool isUnsignedType(clang::QualType Type) {
  return Type->isUnsignedIntegerOrEnumerationType() && !Type->isBooleanType();
}

bool BitwiseUnsignedOperandsVisitor::VisitUnaryOperator(
    const clang::UnaryOperator *UO) {
  if (UO->getOpcode() == clang::UO_Not &&
      !isUnsignedType(UO->getSubExpr()->getType())) {

    return !AD.reportWarning(UO->getSubExpr()->getBeginLoc(),
                             AutocheckWarnings::bitwiseOperandNotUnsigned)
                .limitReached();
  }

  return true;
}

bool BitwiseUnsignedOperandsVisitor::VisitBinaryOperator(
    const clang::BinaryOperator *BO) {
  switch (BO->getOpcode()) {
  case clang::BO_And:
  case clang::BO_AndAssign:
  case clang::BO_Or:
  case clang::BO_OrAssign:
  case clang::BO_Xor:
  case clang::BO_XorAssign:
  case clang::BO_Shl:
  case clang::BO_ShlAssign:
  case clang::BO_Shr:
  case clang::BO_ShrAssign:
    if (!isUnsignedType(BO->getLHS()->IgnoreImpCasts()->getType())) {
      return !AD.reportWarning(BO->getLHS()->getBeginLoc(),
                               AutocheckWarnings::bitwiseOperandNotUnsigned)
                  .limitReached();
    }
    if (!isUnsignedType(BO->getRHS()->IgnoreImpCasts()->getType())) {
      return !AD.reportWarning(BO->getRHS()->getBeginLoc(),
                               AutocheckWarnings::bitwiseOperandNotUnsigned)
                  .limitReached();
    }
    break;
  default:
    return true;
  }

  return true;
}

/* Implementation of CommaOperatorVisitor */

CommaOperatorVisitor::CommaOperatorVisitor(AutocheckDiagnostic &AD) : AD(AD) {}

bool CommaOperatorVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::commaOperatorUsed);
}

bool CommaOperatorVisitor::VisitBinaryOperator(
    const clang::BinaryOperator *BO) {
  if (BO->getOpcode() == clang::BO_Comma) {
    return !AD.reportWarning(BO->getOperatorLoc(),
                             AutocheckWarnings::commaOperatorUsed)
                .limitReached();
  }

  return true;
}

/* Implementation of ExpressionsVisitor */

ExpressionsVisitor::ExpressionsVisitor(AutocheckDiagnostic &AD,
                                       clang::ASTContext &AC)
    : AD(AD) {
  const AutocheckContext &Context = AD.GetContext();
  if (ImplicitlyCapturedVarVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<ImplicitlyCapturedVarVisitor>(AD));
  if (LambdaReturnTypeVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<LambdaReturnTypeVisitor>(AD));
  if (NestedLambdaVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<NestedLambdaVisitor>(AD));
  if (IncDecOpMixedVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<IncDecOpMixedVisitor>(AD, AC));
  if (TernaryOpSubExprVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<TernaryOpSubExprVisitor>(AD, AC));
  if (AssignmentOpSubExprVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<AssignmentOpSubExprVisitor>(AD, AC));
  if (StdMoveUsedConstVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<StdMoveUsedConstVisitor>(AD));
  if (CharacterHandlingFunctionsVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<CharacterHandlingFunctionsVisitor>(AD));
  if (StdFunctionUsed::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<StdFunctionUsed>(AD));
  if (ImplicitBitwiseBinOpConversionVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<ImplicitBitwiseBinOpConversionVisitor>(AD, AC));
  if (SizeofSideEffectVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<SizeofSideEffectVisitor>(AD, AC));
  if (ConditionalOpVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<ConditionalOpVisitor>(AD));
  if (LogicalOpBoolOperandVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<LogicalOpBoolOperandVisitor>(AD));
  if (UnaryMinusOnUnsignedVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<UnaryMinusOnUnsignedVisitor>(AD));
  if (ExceptionObjectPtrVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<ExceptionObjectPtrVisitor>(AD));
  if (ExceptionObjectNullVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<ExceptionObjectNullVisitor>(AD));
  if (BitwiseUnsignedOperandsVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<BitwiseUnsignedOperandsVisitor>(AD));
  if (CommaOperatorVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<CommaOperatorVisitor>(AD));
}

void ExpressionsVisitor::run(clang::TranslationUnitDecl *TUD) {
  if (!AllVisitors.empty())
    this->TraverseDecl(TUD);
}

bool ExpressionsVisitor::TraverseDecl(clang::Decl *D) {
  if (!D) // Can be null if code had errors.
    return true;

  clang::SourceLocation Loc = D->getBeginLoc();

  if (Loc.isInvalid() || appropriateHeaderLocation(AD, Loc)) {
    clang::RecursiveASTVisitor<ExpressionsVisitor>::TraverseDecl(D);
  }
  return true;
}

bool ExpressionsVisitor::TraverseStmt(clang::Stmt *S) {
  AllVisitors.remove_if([S](std::unique_ptr<ExpressionsVisitorInterface> &V) {
    return !V->PreTraverseStmt(S);
  });

  clang::RecursiveASTVisitor<ExpressionsVisitor>::TraverseStmt(S);

  AllVisitors.remove_if([S](std::unique_ptr<ExpressionsVisitorInterface> &V) {
    return !V->PostTraverseStmt(S);
  });

  return true;
}

bool ExpressionsVisitor::VisitLambdaExpr(const clang::LambdaExpr *LE) {
  AllVisitors.remove_if([LE](std::unique_ptr<ExpressionsVisitorInterface> &V) {
    return !V->VisitLambdaExpr(LE);
  });
  return true;
}

bool ExpressionsVisitor::VisitUnaryOperator(const clang::UnaryOperator *UO) {
  AllVisitors.remove_if([UO](std::unique_ptr<ExpressionsVisitorInterface> &V) {
    return !V->VisitUnaryOperator(UO);
  });
  return true;
}

bool ExpressionsVisitor::VisitConditionalOperator(
    const clang::ConditionalOperator *CO) {
  AllVisitors.remove_if([CO](std::unique_ptr<ExpressionsVisitorInterface> &V) {
    return !V->VisitConditionalOperator(CO);
  });
  return true;
}

bool ExpressionsVisitor::VisitBinaryOperator(const clang::BinaryOperator *BO) {
  AllVisitors.remove_if([BO](std::unique_ptr<ExpressionsVisitorInterface> &V) {
    return !V->VisitBinaryOperator(BO);
  });
  return true;
}

bool ExpressionsVisitor::VisitReturnStmt(const clang::ReturnStmt *RS) {
  AllVisitors.remove_if([RS](std::unique_ptr<ExpressionsVisitorInterface> &V) {
    return !V->VisitReturnStmt(RS);
  });
  return true;
}

bool ExpressionsVisitor::VisitCallExpr(const clang::CallExpr *CE) {
  AllVisitors.remove_if([CE](std::unique_ptr<ExpressionsVisitorInterface> &V) {
    return !V->VisitCallExpr(CE);
  });
  return true;
}

bool ExpressionsVisitor::VisitUnaryExprOrTypeTraitExpr(
    const clang::UnaryExprOrTypeTraitExpr *UOTTE) {
  AllVisitors.remove_if(
      [UOTTE](std::unique_ptr<ExpressionsVisitorInterface> &V) {
        return !V->VisitUnaryExprOrTypeTraitExpr(UOTTE);
      });
  return true;
}

bool ExpressionsVisitor::VisitCXXThrowExpr(const clang::CXXThrowExpr *TE) {
  AllVisitors.remove_if([TE](std::unique_ptr<ExpressionsVisitorInterface> &V) {
    return !V->VisitCXXThrowExpr(TE);
  });
  return true;
}

} // namespace autocheck
