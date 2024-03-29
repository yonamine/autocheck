//===--- ConversionsVisitor.cpp - Visitor for conversion related checks ---===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file implements the ConversionsVisitor class and all classes derived
// from ConversionsVisitorInterface.
//
//===----------------------------------------------------------------------===//

#include "AST/ConversionsVisitor.h"

#include "Diagnostics/AutocheckDiagnostic.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/QualTypeNames.h"
#include "clang/Basic/SourceManager.h"
#include <cctype>

namespace autocheck {

/* Implementation of ConversionsVisitorInterface */

ConversionsVisitorInterface::~ConversionsVisitorInterface() {}

using CVI = ConversionsVisitorInterface;
bool CVI::PreTraverseInitListExpr(const clang::InitListExpr *) { return true; }
bool CVI::PostTraverseInitListExpr(const clang::InitListExpr *) { return true; }
bool CVI::VisitCastExpr(const clang::CastExpr *) { return true; }
bool CVI::VisitBinaryOperator(const clang::BinaryOperator *) { return true; }
bool CVI::VisitUnaryOperator(const clang::UnaryOperator *) { return true; }
bool CVI::VisitCXXOperatorCallExpr(const clang::CXXOperatorCallExpr *) {
  return true;
}
bool CVI::VisitImplicitCastExpr(const clang::ImplicitCastExpr *) {
  return true;
}
bool CVI::VisitCallExpr(const clang::CallExpr *) { return true; }
bool CVI::VisitCXXConstructExpr(const clang::CXXConstructExpr *) {
  return true;
}
bool CVI::VisitArraySubscriptExpr(const clang::ArraySubscriptExpr *) {
  return true;
}
bool CVI::VisitVarDecl(const clang::VarDecl *) { return true; }
bool CVI::VisitCXXConstCastExpr(const clang::CXXConstCastExpr *) {
  return true;
}

/* Implementation of InvalidBoolExpressionVisitor */

InvalidBoolExpressionVisitor::InvalidBoolExpressionVisitor(
    AutocheckDiagnostic &AD)
    : AD(AD) {
  AllowedUnaryOps.insert(clang::UO_LNot);
  AllowedUnaryOps.insert(clang::UO_AddrOf);

  AllowedBinaryOps.insert(clang::BO_Assign);
  AllowedBinaryOps.insert(clang::BO_EQ);
  AllowedBinaryOps.insert(clang::BO_NE);
  AllowedBinaryOps.insert(clang::BO_LAnd);
  AllowedBinaryOps.insert(clang::BO_LOr);
}

bool InvalidBoolExpressionVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::invalidBoolExpression);
}

bool InvalidBoolExpressionVisitor::VisitBinaryOperator(
    const clang::BinaryOperator *BO) {
  if ((isExprBooleanType(BO->getLHS()) || isExprBooleanType(BO->getRHS())) &&
      !AllowedBinaryOps.contains(BO->getOpcode())) {
    return !AD.reportWarning(BO->getOperatorLoc(),
                             AutocheckWarnings::invalidBoolExpression,
                             BO->getSourceRange())
                .limitReached();
  }

  return true;
}

bool InvalidBoolExpressionVisitor::VisitUnaryOperator(
    const clang::UnaryOperator *UO) {
  if (isExprBooleanType(UO->getSubExpr()) &&
      !AllowedUnaryOps.contains(UO->getOpcode())) {
    return !AD.reportWarning(UO->getOperatorLoc(),
                             AutocheckWarnings::invalidBoolExpression,
                             UO->getSourceRange())
                .limitReached();
  }

  return true;
}

bool InvalidBoolExpressionVisitor::isExprBooleanType(
    const clang::Expr *E) const {
  const clang::QualType &ExprType = E->IgnoreImpCasts()->getType();
  return ExprType->isBooleanType();
}

/* Implementation of InvalidEnumExpressionVisitor */

InvalidEnumExpressionVisitor::InvalidEnumExpressionVisitor(
    AutocheckDiagnostic &AD)
    : AD(AD) {
  AllowedUnaryOps.insert(clang::UO_AddrOf);

  AllowedBinaryOps.insert(clang::BO_EQ);
  AllowedBinaryOps.insert(clang::BO_NE);
  AllowedBinaryOps.insert(clang::BO_LT);
  AllowedBinaryOps.insert(clang::BO_LE);
  AllowedBinaryOps.insert(clang::BO_GT);
  AllowedBinaryOps.insert(clang::BO_GE);
  AllowedBinaryOps.insert(clang::BO_Assign);

  AllowedOperators.insert(clang::OO_EqualEqual);
  AllowedOperators.insert(clang::OO_ExclaimEqual);
  AllowedOperators.insert(clang::OO_Less);
  AllowedOperators.insert(clang::OO_LessEqual);
  AllowedOperators.insert(clang::OO_Greater);
  AllowedOperators.insert(clang::OO_GreaterEqual);
  AllowedOperators.insert(clang::OO_Equal);
  AllowedOperators.insert(clang::OO_Subscript);
  AllowedOperators.insert(clang::OO_Amp);
}

bool InvalidEnumExpressionVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::invalidEnumExpression);
}

bool InvalidEnumExpressionVisitor::VisitBinaryOperator(
    const clang::BinaryOperator *BO) {
  if ((isExprEnumerationType(BO->getLHS()) ||
       isExprEnumerationType(BO->getRHS())) &&
      !AllowedBinaryOps.contains(BO->getOpcode())) {
    return !AD.reportWarning(BO->getOperatorLoc(),
                             AutocheckWarnings::invalidEnumExpression,
                             BO->getSourceRange())
                .limitReached();
  }
  return true;
}

bool InvalidEnumExpressionVisitor::VisitUnaryOperator(
    const clang::UnaryOperator *UO) {
  if (isExprEnumerationType(UO->getSubExpr()) &&
      !AllowedUnaryOps.contains(UO->getOpcode())) {
    return !AD.reportWarning(UO->getOperatorLoc(),
                             AutocheckWarnings::invalidEnumExpression,
                             UO->getSourceRange())
                .limitReached();
  }
  return true;
}

bool InvalidEnumExpressionVisitor::VisitCXXOperatorCallExpr(
    const clang::CXXOperatorCallExpr *E) {
  bool hasEnumerationType = false;
  for (const clang::Expr *Arg : E->arguments())
    if (isExprEnumerationType(Arg)) {
      hasEnumerationType = true;
      break;
    }
  if (hasEnumerationType && !AllowedOperators.contains(E->getOperator())) {
    return !AD.reportWarning(E->getOperatorLoc(),
                             AutocheckWarnings::invalidEnumExpression,
                             E->getSourceRange())
                .limitReached();
  }
  return true;
}

bool InvalidEnumExpressionVisitor::isExprEnumerationType(
    const clang::Expr *E) const {
  return E->IgnoreImpCasts()->getType()->isEnumeralType();
}

/* Implementation of InvalidCharExpressionVisitor */

InvalidCharExpressionVisitor::InvalidCharExpressionVisitor(
    AutocheckDiagnostic &AD, clang::ASTContext &AC)
    : AD(AD), AC(AC) {
  AllowedUnaryOps.insert(clang::UO_AddrOf);

  AllowedBinaryOps.insert(clang::BO_Assign);
  AllowedBinaryOps.insert(clang::BO_EQ);
  AllowedBinaryOps.insert(clang::BO_NE);

  ExceptionOps.insert(clang::BO_LT);
  ExceptionOps.insert(clang::BO_LE);
  ExceptionOps.insert(clang::BO_GT);
  ExceptionOps.insert(clang::BO_GE);
}

bool InvalidCharExpressionVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::invalidCharExpression);
}

bool InvalidCharExpressionVisitor::VisitBinaryOperator(
    const clang::BinaryOperator *BO) {
  const clang::Expr *LHS = BO->getLHS();
  const clang::Expr *RHS = BO->getRHS();
  const clang::BinaryOperator::Opcode BinOpCode = BO->getOpcode();

  if ((isExprCharType(LHS) || isExprCharType(RHS)) &&
      !AllowedBinaryOps.contains(BinOpCode)) {
    const clang::Expr *LHSI = LHS->IgnoreImpCasts();
    const clang::Expr *RHSI = RHS->IgnoreImpCasts();
    const clang::CharacterLiteral *LCL =
        llvm::dyn_cast_if_present<clang::CharacterLiteral>(LHSI);
    const clang::CharacterLiteral *RCL =
        llvm::dyn_cast_if_present<clang::CharacterLiteral>(RHSI);

    // Exception: The binary + operator may be used to add an integral value in
    // the range 0 to 9 to '0'.
    if (BinOpCode == clang::BO_Add) {
      if (LCL && LCL->getValue() == '0' && isIntLessThan10(RHSI))
        return true;
      if (RCL && RCL->getValue() == '0' && isIntLessThan10(LHSI))
        return true;
    }

    // Exception: The binary - operator may be used to subtract character '0'.
    if (BinOpCode == clang::BO_Sub)
      if (RCL && RCL->getValue() == '0')
        return true;

    // Exception: The relational operators < , <= , > , >= may be used to
    // determine if a character (or wide character) represents a digit.
    if (ExceptionOps.contains(BinOpCode)) {
      if (LCL && isdigit(LCL->getValue()))
        return true;
      if (RCL && isdigit(RCL->getValue()))
        return true;
    }

    return !AD.reportWarning(BO->getOperatorLoc(),
                             AutocheckWarnings::invalidCharExpression,
                             BO->getSourceRange())
                .limitReached();
  }
  return true;
}

bool InvalidCharExpressionVisitor::VisitUnaryOperator(
    const clang::UnaryOperator *UO) {
  if (isExprCharType(UO->getSubExpr()) &&
      !AllowedUnaryOps.contains(UO->getOpcode())) {
    return !AD.reportWarning(UO->getOperatorLoc(),
                             AutocheckWarnings::invalidCharExpression,
                             UO->getSourceRange())
                .limitReached();
  }
  return true;
}

bool InvalidCharExpressionVisitor::isExprCharType(const clang::Expr *E) const {
  const clang::QualType &Type = E->IgnoreImpCasts()->getType();
  if (Type->isCharType() || Type->isWideCharType() || Type->isChar32Type() ||
      Type->isChar16Type() || Type->isChar8Type()) {
    const std::string TypeName = Type.getAsString();
    if (!(TypeName == "std::uint8_t" || TypeName == "std::int8_t" ||
          TypeName == "uint8_t" || TypeName == "int8_t")) {
      return true;
    }
  }
  return false;
}

// Check if given Expr is an IntegerLiteral in 0-9 range.
bool InvalidCharExpressionVisitor::isIntLessThan10(const clang::Expr *E) const {
  const clang::IntegerLiteral *IL =
      llvm::dyn_cast_if_present<clang::IntegerLiteral>(E);
  if (!IL)
    return false;
  clang::Expr::EvalResult Result;
  if (IL->EvaluateAsInt(Result, AC))
    return Result.Val.getInt() >= 0 && Result.Val.getInt() < 10;
  return false;
}

/* Implementation of ImplicitFloatIntegralConversionVisitor */

ImplicitFloatIntegralConversionVisitor::ImplicitFloatIntegralConversionVisitor(
    AutocheckDiagnostic &AD)
    : AD(AD) {}

bool ImplicitFloatIntegralConversionVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::implicitFloatIntegralConversion);
}

bool ImplicitFloatIntegralConversionVisitor::VisitImplicitCastExpr(
    const clang::ImplicitCastExpr *ICE) {
  // Skip explicit casts.
  if (!ICE->isPartOfExplicitCast() &&
      !llvm::dyn_cast_if_present<clang::ExplicitCastExpr>(ICE->getSubExpr())) {
    const clang::CastKind &Kind = ICE->getCastKind();
    if (Kind == clang::CK_FloatingToIntegral ||
        Kind == clang::CK_IntegralToFloating) {
      return !AD.reportWarning(
                    ICE->getBeginLoc(),
                    AutocheckWarnings::implicitFloatIntegralConversion,
                    ICE->getSourceRange())
                  .limitReached();
    }
  }
  return true;
}

/* Implementation of ImplicitSizeReductionConversionVisitor */

ImplicitSizeReductionConversionVisitor::ImplicitSizeReductionConversionVisitor(
    AutocheckDiagnostic &AD, clang::ASTContext &ASTCtx)
    : AD(AD), ASTCtx(ASTCtx) {}

bool ImplicitSizeReductionConversionVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::implicitSizeReductionConversion);
}

bool ImplicitSizeReductionConversionVisitor::VisitImplicitCastExpr(
    const clang::ImplicitCastExpr *ICE) {
  // Skip explicit casts.
  if (!ICE->isPartOfExplicitCast() &&
      !llvm::dyn_cast_if_present<clang::ExplicitCastExpr>(ICE->getSubExpr())) {

    const clang::CastKind Kind = ICE->getCastKind();
    if (Kind == clang::CK_IntegralCast || Kind == clang::CK_FloatingCast) {
      if (isReducedSizeFromSubExpr(ICE)) {
        return !AD.reportWarning(
                      ICE->getBeginLoc(),
                      AutocheckWarnings::implicitSizeReductionConversion,
                      ICE->getSourceRange())
                    .limitReached();
      }
    }
  }
  return true;
}

bool ImplicitSizeReductionConversionVisitor::isReducedSizeFromSubExpr(
    const clang::ImplicitCastExpr *ICE) const {
  return (ASTCtx.getTypeSize(ICE->getType()) <
          ASTCtx.getTypeSize(ICE->getSubExpr()->getType()));
}

/* Implementation of ImplicitBitwiseResultConversionVisitor */

ImplicitBitwiseResultConversionVisitor::ImplicitBitwiseResultConversionVisitor(
    AutocheckDiagnostic &AD, clang::ASTContext &ASTCtx)
    : AD(AD), ASTCtx(ASTCtx) {}

bool ImplicitBitwiseResultConversionVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::implicitBitwiseResultConversion);
}

bool ImplicitBitwiseResultConversionVisitor::VisitUnaryOperator(
    const clang::UnaryOperator *UO) {
  // Check if this is bitwise operator ~.
  if (UO->getOpcode() == clang::UO_Not) {
    const clang::QualType Type = UO->getSubExpr()->IgnoreImpCasts()->getType();
    // Check if underlying type is unsigned char or unsigned short and if this
    // expression is not being explicitly cast.
    if (isUnsignedCharOrShort(Type) && !isBeingExplicityCastToType(UO, Type)) {
      return !AD.reportWarning(
                    UO->getOperatorLoc(),
                    AutocheckWarnings::implicitBitwiseResultConversion,
                    UO->getSourceRange())
                  .limitReached();
    }
  }
  return true;
}

bool ImplicitBitwiseResultConversionVisitor::VisitBinaryOperator(
    const clang::BinaryOperator *BO) {
  // Check if this is bitwise operator <<.
  if (BO->getOpcode() == clang::BO_Shl) {
    const clang::QualType Type = BO->getLHS()->IgnoreImpCasts()->getType();
    // Check if underlying type is unsigned char or unsigned short and if this
    // expression is not being explicitly cast.
    if (isUnsignedCharOrShort(Type) && !isBeingExplicityCastToType(BO, Type)) {
      return !AD.reportWarning(
                    BO->getOperatorLoc(),
                    AutocheckWarnings::implicitBitwiseResultConversion,
                    BO->getSourceRange())
                  .limitReached();
    }
  }
  return true;
}

bool ImplicitBitwiseResultConversionVisitor::isUnsignedCharOrShort(
    const clang::QualType &Type) const {
  const std::string TypeName = Type.getDesugaredType(ASTCtx).getAsString();
  return (TypeName == "unsigned char" || TypeName == "unsigned short");
}

bool ImplicitBitwiseResultConversionVisitor::isBeingExplicityCastToType(
    const clang::Expr *E, const clang::QualType &Type) const {
  const auto &Parents = ASTCtx.getParents(*E);
  if (Parents.size() != 1)
    return true;

  if (const clang::ExplicitCastExpr *ECE =
          Parents[0].get<clang::ExplicitCastExpr>()) {
    return (Type.getDesugaredType(ASTCtx) ==
            ECE->getType().getDesugaredType(ASTCtx));
  }
  if (const clang::ImplicitCastExpr *ICE =
          Parents[0].get<clang::ImplicitCastExpr>()) {
    return (ICE->isPartOfExplicitCast() &&
            Type.getDesugaredType(ASTCtx) ==
                ICE->getType().getDesugaredType(ASTCtx));
  }
  return false;
}

/* Implementation of ArrayDecaysToPointerVisitor */

ArrayDecaysToPointerVisitor::ArrayDecaysToPointerVisitor(
    AutocheckDiagnostic &AD, clang::ASTContext &ASTCtx)
    : AD(AD), ASTCtx(ASTCtx) {}

bool ArrayDecaysToPointerVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::arrayDecaysToPointer);
}

bool ArrayDecaysToPointerVisitor::VisitCallExpr(const clang::CallExpr *CE) {
  for (const clang::Expr *Arg : CE->arguments())
    if (!checkArgument(Arg))
      return false;
  return true;
}

bool ArrayDecaysToPointerVisitor::VisitCXXConstructExpr(
    const clang::CXXConstructExpr *CCE) {
  for (const clang::Expr *Arg : CCE->arguments())
    if (!checkArgument(Arg))
      return false;
  return true;
}

bool ArrayDecaysToPointerVisitor::checkArgument(const clang::Expr *Arg) const {
  const clang::ImplicitCastExpr *PICE =
      llvm::dyn_cast_if_present<const clang::ImplicitCastExpr>(Arg);
  if (!PICE)
    return true;
  if (PICE->getCastKind() == clang::CastKind::CK_DerivedToBase)
    // Skip since this case is handled by VisitImplicitCastExpr.
    return true;

  const clang::Expr *E = Arg->IgnoreParenNoopCasts(ASTCtx);
  if (const clang::ImplicitCastExpr *ICE =
          llvm::dyn_cast_if_present<const clang::ImplicitCastExpr>(E))
    if (ICE->getCastKind() == clang::CastKind::CK_ArrayToPointerDecay) {
      const clang::Expr *SubExpr = ICE->getSubExpr();
      if (llvm::dyn_cast_if_present<const clang::StringLiteral>(SubExpr))
        return true;
      // TODO: Instead of PredefinedExpr, maybe something else might appear in
      // AST between StringLiteral and ImplicitCastExpr.
      if (llvm::dyn_cast_if_present<const clang::PredefinedExpr>(SubExpr)) {
        clang::Expr::const_child_iterator IChild = SubExpr->child_begin();
        if (llvm::dyn_cast_if_present<const clang::StringLiteral>(*IChild))
          return true;
      }
      return !AD.reportWarning(Arg->getBeginLoc(),
                               AutocheckWarnings::arrayDecaysToPointer, 0,
                               SubExpr->getType().getAsString(),
                               ICE->getType().getAsString())
                  .limitReached();
    }
  return true;
}

bool ArrayDecaysToPointerVisitor::VisitArraySubscriptExpr(
    const clang::ArraySubscriptExpr *ASE) {
  const clang::Expr *ArrayBase = ASE->getBase()->IgnoreCasts();
  const clang::Expr *ArrayIndex = ASE->getIdx();
  if (!ArrayBase->getType()->isPointerType())
    return true;
  const clang::DeclRefExpr *DRE =
      llvm::dyn_cast_if_present<const clang::DeclRefExpr>(ArrayBase);
  if (!DRE)
    return true;
  const clang::VarDecl *VD =
      llvm::dyn_cast_if_present<const clang::VarDecl>(DRE->getDecl());
  if (!VD)
    return true;
  if (!llvm::dyn_cast_if_present<clang::ParmVarDecl>(VD))
    return true;
  if (!ArrayIndex->isValueDependent() && !ArrayIndex->isEvaluatable(ASTCtx))
    return true;
  if (!ArrayIndex->isValueDependent() &&
      ArrayIndex->EvaluateKnownConstInt(ASTCtx).getExtValue() == 0)
    return true;
  return !AD.reportWarning(ASE->getEndLoc(),
                           AutocheckWarnings::arrayDecaysToPointer, 1)
              .limitReached();
}

bool ArrayDecaysToPointerVisitor::VisitImplicitCastExpr(
    const clang::ImplicitCastExpr *ICE) {
  const auto &Parents = ASTCtx.getParents(*ICE);
  if (Parents.size() != 1)
    return true;
  const clang::InitListExpr *ILE = Parents[0].get<clang::InitListExpr>();
  if (ILE && std::distance(ILE->child_begin(), ILE->child_end()) == 1)
    // Handled by VisitVarDecl.
    return true;

  return checkDecayToBaseClassPtr(ICE);
}

bool ArrayDecaysToPointerVisitor::VisitVarDecl(const clang::VarDecl *VD) {
  const clang::Expr *Init = VD->getInit();

  const clang::InitListExpr *ILE =
      llvm::dyn_cast_if_present<clang::InitListExpr>(Init);
  if (!ILE)
    return true;

  if (std::distance(ILE->child_begin(), ILE->child_end()) != 1)
    // Handled by VisitImplicitCastExpr.
    return true;

  const clang::ImplicitCastExpr *E =
      llvm::dyn_cast_if_present<const clang::ImplicitCastExpr>(
          *ILE->child_begin());
  if (!E)
    return true;

  return checkDecayToBaseClassPtr(E);
}

bool ArrayDecaysToPointerVisitor::checkDecayToBaseClassPtr(
    const clang::ImplicitCastExpr *ICE) const {
  if (ICE->getCastKind() != clang::CastKind::CK_DerivedToBase)
    return true;
  const clang::ImplicitCastExpr *SE =
      llvm::dyn_cast_if_present<const clang::ImplicitCastExpr>(
          ICE->getSubExpr());
  if (!SE)
    return true;
  if (SE->getCastKind() != clang::CastKind::CK_ArrayToPointerDecay)
    return true;
  return !AD.reportWarning(ICE->getBeginLoc(),
                           AutocheckWarnings::arrayDecaysToPointer, 2,
                           SE->getSubExpr()->getType().getAsString(),
                           ICE->getType().getAsString())
              .limitReached();
}

/* Implementation of NullToIntegerValueVisitor */

NullToIntegerValueVisitor::NullToIntegerValueVisitor(AutocheckDiagnostic &AD)
    : AD(AD) {}

bool NullToIntegerValueVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::nullToIntegerValue);
}

bool NullToIntegerValueVisitor::VisitCastExpr(const clang::CastExpr *CE) {
  const clang::SourceLocation &Loc = CE->getBeginLoc();
  const clang::Expr *SE = CE->getSubExpr();
  if (!SE)
    return true;
  if (CE->getCastKind() == clang::CK_IntegralCast)
    if (llvm::dyn_cast_if_present<clang::GNUNullExpr>(SE)) {
      return !AD.reportWarning(Loc, AutocheckWarnings::nullToIntegerValue)
                  .limitReached();
    }
  return true;
}

/* Implementation of ZeroToNullPointerVisitor */

ZeroToNullPointerVisitor::ZeroToNullPointerVisitor(AutocheckDiagnostic &AD)
    : AD(AD) {}

bool ZeroToNullPointerVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::zeroToNullPointer);
}

bool ZeroToNullPointerVisitor::VisitCastExpr(const clang::CastExpr *CE) {
  const clang::SourceLocation &Loc = CE->getBeginLoc();
  const clang::Expr *SE = CE->getSubExpr();
  if (!SE)
    return true;
  if (CE->getCastKind() == clang::CK_NullToPointer)
    if (llvm::dyn_cast_if_present<clang::IntegerLiteral>(SE)) {
      return !AD.reportWarning(Loc, AutocheckWarnings::zeroToNullPointer)
                  .limitReached();
    }
  return true;
}

/* Implementation of NullptrOnlyNullPtrConstVisitor */

NullptrOnlyNullPtrConstVisitor::NullptrOnlyNullPtrConstVisitor(
    AutocheckDiagnostic &AD)
    : AD(AD) {}

bool NullptrOnlyNullPtrConstVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::nullptrOnlyNullPtrConst);
}

bool NullptrOnlyNullPtrConstVisitor::VisitCastExpr(const clang::CastExpr *CE) {
  const clang::SourceLocation &Loc = CE->getBeginLoc();
  const clang::Expr *SE = CE->getSubExpr();
  if (!SE)
    return true;
  if (CE->getCastKind() == clang::CK_NullToPointer)
    if (!llvm::dyn_cast_if_present<clang::CXXNullPtrLiteralExpr>(SE)) {
      return !AD.reportWarning(Loc, AutocheckWarnings::nullptrOnlyNullPtrConst)
                  .limitReached();
    }
  return true;
}

/* Implementation of CastPtrToIntegralVisitor */

CastPtrToIntegralVisitor::CastPtrToIntegralVisitor(AutocheckDiagnostic &AD)
    : AD(AD) {}

bool CastPtrToIntegralVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::castPtrToIntegralType);
}

bool CastPtrToIntegralVisitor::VisitCastExpr(const clang::CastExpr *CE) {
  if (CE->getCastKind() == clang::CK_PointerToIntegral) {
    return !AD.reportWarning(CE->getBeginLoc(),
                             AutocheckWarnings::castPtrToIntegralType)
                .limitReached();
  }
  return true;
}

/* Implementation of CVDiscardCastVisitor */

CVDiscardCastVisitor::CVDiscardCastVisitor(AutocheckDiagnostic &AD) : AD(AD) {}

bool CVDiscardCastVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::castRemovesQual);
}

bool CVDiscardCastVisitor::VisitCXXConstCastExpr(
    const clang::CXXConstCastExpr *CCCE) {
  // This only handles const casts. Other CV discarding casts are handled by
  // -Wcast-qual in AutocheckDiagnosticConsumer.
  clang::QualType FromType = CCCE->getSubExpr()->getType();
  clang::QualType ToType = CCCE->getType();
  if (FromType->isPointerType()) {
    FromType = FromType->getPointeeType();
    ToType = ToType->getPointeeType();
  } else if (FromType->isReferenceType()) {
    FromType = FromType.getNonReferenceType();
    ToType = ToType.getNonReferenceType();
  } else {
    // We are only interested in pointer and referece types.
    return true;
  }

  bool DropsConstQual = false;
  bool DropsVolatileQual = false;
  if (FromType.isConstQualified() && !ToType.isConstQualified()) {
    DropsConstQual = true;
  }
  if (FromType.isVolatileQualified() && !ToType.isVolatileQualified()) {
    DropsVolatileQual = true;
  }

  // Match parameters for clang::diag::cast_qual diagnostic.
  int Qualifiers = -1;
  if (DropsConstQual && DropsVolatileQual)
    Qualifiers = 0;
  else if (DropsConstQual)
    Qualifiers = 1;
  else if (DropsVolatileQual)
    Qualifiers = 2;

  if (Qualifiers != -1) {
    bool stopVisitor = AD.reportWarning(CCCE->getBeginLoc(),
                                        AutocheckWarnings::castRemovesQual)
                           .limitReached();

    AD.reportWarning(
        CCCE->getBeginLoc(), AutocheckWarnings::noteCastRemovesQual, 0,
        CCCE->getSubExpr()->getType(), CCCE->getType(), Qualifiers);

    return !stopVisitor;
  }

  return true;
}

/* Implementation of ImpcastChangesSignednessVisitor */

ImpcastChangesSignednessVisitor::ImpcastChangesSignednessVisitor(
    AutocheckDiagnostic &AD, const clang::ASTContext &AC)
    : AD(AD), AC(AC) {}

bool ImpcastChangesSignednessVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::impcastChangesSignedness);
}

static bool isIntegralCast(const clang::CastExpr *CE) {
  return CE->getCastKind() == clang::CK_IntegralCast &&
         !CE->getSubExpr()->getType()->isBooleanType() &&
         !CE->getSubExpr()->getType()->isEnumeralType();
}

bool ImpcastChangesSignednessVisitor::PreTraverseInitListExpr(
    const clang::InitListExpr *ILE) {
  if ((ILE->isSyntacticForm() && !ILE->isSemanticForm()) ||
      (!IsInsideSyntacticILEForm.empty() && IsInsideSyntacticILEForm.back()))
    IsInsideSyntacticILEForm.emplace_back(true);
  else
    IsInsideSyntacticILEForm.emplace_back(false);
  return true;
}

bool ImpcastChangesSignednessVisitor::PostTraverseInitListExpr(
    const clang::InitListExpr *ILE) {
  IsInsideSyntacticILEForm.pop_back();
  return true;
}

bool ImpcastChangesSignednessVisitor::VisitImplicitCastExpr(
    const clang::ImplicitCastExpr *ICE) {
  // We are only interested in the sematic form since the syntactic form doesn't
  // generate implcit casts.
  if (!IsInsideSyntacticILEForm.empty() && IsInsideSyntacticILEForm.back())
    return true;

  if (!ICE->isPartOfExplicitCast() && isIntegralCast(ICE) &&
      ICE->getType()->isUnsignedIntegerType() !=
          ICE->getSubExpr()->getType()->isUnsignedIntegerType()) {
    bool stopVisitor =
        AD.reportWarning(ICE->getBeginLoc(),
                         AutocheckWarnings::impcastChangesSignedness)
            .limitReached();

    const clang::PrintingPolicy &Policy = AC.getPrintingPolicy();
    AD.reportWarning(
        ICE->getBeginLoc(), AutocheckWarnings::noteImpcastChangesSignedness,
        clang::TypeName::getFullyQualifiedName(ICE->getSubExpr()->getType(), AC,
                                               Policy),
        clang::TypeName::getFullyQualifiedName(ICE->getType(), AC, Policy));

    return !stopVisitor;
  }

  return true;
}

/* Implementation of ConversionsVisitor */

ConversionsVisitor::ConversionsVisitor(AutocheckDiagnostic &AD,
                                       clang::ASTContext &AC)
    : AD(AD) {
  const AutocheckContext &Context = AD.GetContext();
  if (InvalidBoolExpressionVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<InvalidBoolExpressionVisitor>(AD));
  if (InvalidEnumExpressionVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<InvalidEnumExpressionVisitor>(AD));
  if (InvalidCharExpressionVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<InvalidCharExpressionVisitor>(AD, AC));
  if (ImplicitFloatIntegralConversionVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<ImplicitFloatIntegralConversionVisitor>(AD));
  if (ImplicitSizeReductionConversionVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<ImplicitSizeReductionConversionVisitor>(AD, AC));
  if (ImplicitBitwiseResultConversionVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<ImplicitBitwiseResultConversionVisitor>(AD, AC));
  if (ArrayDecaysToPointerVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<ArrayDecaysToPointerVisitor>(AD, AC));
  if (NullToIntegerValueVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<NullToIntegerValueVisitor>(AD));
  if (ZeroToNullPointerVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<ZeroToNullPointerVisitor>(AD));
  if (NullptrOnlyNullPtrConstVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<NullptrOnlyNullPtrConstVisitor>(AD));
  if (CastPtrToIntegralVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<CastPtrToIntegralVisitor>(AD));
  if (CVDiscardCastVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<CVDiscardCastVisitor>(AD));
  if (ImpcastChangesSignednessVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<ImpcastChangesSignednessVisitor>(AD, AC));
}

void ConversionsVisitor::run(clang::TranslationUnitDecl *TUD) {
  if (!AllVisitors.empty())
    this->TraverseDecl(TUD);
}

bool ConversionsVisitor::TraverseDecl(clang::Decl *D) {
  if (!D) // Can be null if code had errors.
    return true;

  clang::SourceLocation Loc = D->getBeginLoc();

  if (Loc.isInvalid() || appropriateHeaderLocation(AD, Loc)) {
    clang::RecursiveASTVisitor<ConversionsVisitor>::TraverseDecl(D);
  }
  return true;
}

bool ConversionsVisitor::TraverseInitListExpr(clang::InitListExpr *ILE) {
  if (!ILE) // Can be null if code had errors.
    return true;

  clang::SourceLocation Loc = ILE->getBeginLoc();

  if (Loc.isInvalid() || appropriateHeaderLocation(AD, Loc)) {
    AllVisitors.remove_if(
        [ILE](std::unique_ptr<ConversionsVisitorInterface> &V) {
          return !V->PreTraverseInitListExpr(ILE);
        });

    RecursiveASTVisitor<ConversionsVisitor>::TraverseInitListExpr(ILE);

    AllVisitors.remove_if(
        [ILE](std::unique_ptr<ConversionsVisitorInterface> &V) {
          return !V->PostTraverseInitListExpr(ILE);
        });
  }
  return true;
}

bool ConversionsVisitor::VisitCastExpr(const clang::CastExpr *E) {
  AllVisitors.remove_if([E](std::unique_ptr<ConversionsVisitorInterface> &V) {
    return !V->VisitCastExpr(E);
  });
  return true;
}

bool ConversionsVisitor::VisitBinaryOperator(const clang::BinaryOperator *BO) {
  AllVisitors.remove_if([BO](std::unique_ptr<ConversionsVisitorInterface> &V) {
    return !V->VisitBinaryOperator(BO);
  });
  return true;
}

bool ConversionsVisitor::VisitUnaryOperator(const clang::UnaryOperator *UO) {
  AllVisitors.remove_if([UO](std::unique_ptr<ConversionsVisitorInterface> &V) {
    return !V->VisitUnaryOperator(UO);
  });
  return true;
}

bool ConversionsVisitor::VisitCXXOperatorCallExpr(
    const clang::CXXOperatorCallExpr *E) {
  AllVisitors.remove_if([E](std::unique_ptr<ConversionsVisitorInterface> &V) {
    return !V->VisitCXXOperatorCallExpr(E);
  });
  return true;
}

bool ConversionsVisitor::VisitImplicitCastExpr(
    const clang::ImplicitCastExpr *E) {
  AllVisitors.remove_if([E](std::unique_ptr<ConversionsVisitorInterface> &V) {
    return !V->VisitImplicitCastExpr(E);
  });
  return true;
}

bool ConversionsVisitor::VisitCallExpr(const clang::CallExpr *CE) {
  AllVisitors.remove_if([CE](std::unique_ptr<ConversionsVisitorInterface> &V) {
    return !V->VisitCallExpr(CE);
  });
  return true;
}

bool ConversionsVisitor::VisitCXXConstructExpr(
    const clang::CXXConstructExpr *CCE) {
  AllVisitors.remove_if([CCE](std::unique_ptr<ConversionsVisitorInterface> &V) {
    return !V->VisitCXXConstructExpr(CCE);
  });
  return true;
}

bool ConversionsVisitor::VisitArraySubscriptExpr(
    const clang::ArraySubscriptExpr *ASE) {
  AllVisitors.remove_if([ASE](std::unique_ptr<ConversionsVisitorInterface> &V) {
    return !V->VisitArraySubscriptExpr(ASE);
  });
  return true;
}

bool ConversionsVisitor::VisitVarDecl(const clang::VarDecl *VD) {
  AllVisitors.remove_if([VD](std::unique_ptr<ConversionsVisitorInterface> &V) {
    return !V->VisitVarDecl(VD);
  });
  return true;
}

bool ConversionsVisitor::VisitCXXConstCastExpr(
    const clang::CXXConstCastExpr *CE) {
  AllVisitors.remove_if([CE](std::unique_ptr<ConversionsVisitorInterface> &V) {
    return !V->VisitCXXConstCastExpr(CE);
  });
  return true;
}

} // namespace autocheck
