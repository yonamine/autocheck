//===--- TypesVisitor.cpp - Visitor for type related checks ---------------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file implements the TypesVisitor class and all classes derived from
// TypesVisitorInterface.
//
//===----------------------------------------------------------------------===//

#include "AST/TypesVisitor.h"

#include "Diagnostics/AutocheckDiagnostic.h"

namespace autocheck {

/* Implementation of TypesVisitorInterface */

TypesVisitorInterface::~TypesVisitorInterface() {}

using TVI = TypesVisitorInterface;
bool TVI::VisitImplicitCastExpr(const clang::ImplicitCastExpr *) {
  return true;
}
bool TVI::VisitIfStmt(clang::IfStmt *) { return true; }
bool TVI::VisitWhileStmt(clang::WhileStmt *) { return true; }
bool TVI::VisitForStmt(clang::ForStmt *) { return true; }
bool TVI::VisitDoStmt(clang::DoStmt *) { return true; }
bool TVI::VisitTypeLoc(const clang::TypeLoc &) { return true; }
bool TVI::VisitVarDecl(const clang::VarDecl *) { return true; }
bool TVI::VisitCXXNewExpr(const clang::CXXNewExpr *) { return true; }
bool TVI::VisitCXXTypeidExpr(const clang::CXXTypeidExpr *) { return true; }

/* Implementation of CharStorageVisitor */

CharStorageVisitor::CharStorageVisitor(AutocheckDiagnostic &AD,
                                       clang::ASTContext &AC)
    : AD(AD), AC(AC) {}

bool CharStorageVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::charStorage);
}

bool CharStorageVisitor::VisitImplicitCastExpr(
    const clang::ImplicitCastExpr *ICE) {
  if (ICE->getCastKind() == clang::CK_IntegralCast &&
      isPlainChar(ICE->getType())) {
    return !AD.reportWarning(ICE->getBeginLoc(), AutocheckWarnings::charStorage,
                             ICE->getSourceRange())
                .limitReached();
  }
  return true;
}

bool CharStorageVisitor::isPlainChar(const clang::QualType &Type) const {
  return (Type->isCharType() &&
          Type.getDesugaredType(AC).getAsString() == "char");
}

/* Implementation of SignCharStorageVisitor */

SignCharStorageVisitor::SignCharStorageVisitor(AutocheckDiagnostic &AD,
                                               clang::ASTContext &AC)
    : AD(AD), AC(AC) {}

bool SignCharStorageVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::signCharStorage);
}

bool SignCharStorageVisitor::VisitImplicitCastExpr(
    const clang::ImplicitCastExpr *ICE) {
  if (ICE->getCastKind() == clang::CK_IntegralCast &&
      isUnsignedOrSignedChar(ICE->getType()) &&
      isPlainChar(ICE->getSubExpr()->getType())) {
    return !AD.reportWarning(ICE->getBeginLoc(),
                             AutocheckWarnings::signCharStorage,
                             ICE->getSourceRange())
                .limitReached();
  }

  return true;
}

bool SignCharStorageVisitor::isPlainChar(const clang::QualType &Type) const {
  return (Type->isCharType() &&
          Type.getDesugaredType(AC).getAsString() == "char");
}

bool SignCharStorageVisitor::isUnsignedOrSignedChar(
    const clang::QualType &Type) const {
  if (Type->isCharType()) {
    const std::string TypeName = Type.getDesugaredType(AC).getAsString();
    return (TypeName == "unsigned char" || TypeName == "signed char");
  }
  return false;
}

/* Implementation of ConditionNotBoolVisitor */

ConditionNotBoolVisitor::ConditionNotBoolVisitor(AutocheckDiagnostic &AD)
    : AD(AD), IsBefore(AD.GetSourceManager()) {}

bool ConditionNotBoolVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::contiditionNotBool);
}

bool ConditionNotBoolVisitor::VisitIfStmt(clang::IfStmt *IS) {
  if (!IS->getConditionVariable()) {
    diagnoseCondition(IS->getCond());
  }
  return true;
}

bool ConditionNotBoolVisitor::VisitWhileStmt(clang::WhileStmt *WS) {
  if (!WS->getConditionVariable())
    diagnoseCondition(WS->getCond());
  return true;
}

bool ConditionNotBoolVisitor::VisitForStmt(clang::ForStmt *FS) {
  if (!FS->getConditionVariable())
    diagnoseCondition(FS->getCond());
  return true;
}

bool ConditionNotBoolVisitor::VisitDoStmt(clang::DoStmt *DS) {
  diagnoseCondition(DS->getCond());
  return true;
}

bool ConditionNotBoolVisitor::VisitImplicitCastExpr(
    const clang::ImplicitCastExpr *ICE) {
  if (ConditionSourceRange.isValid() &&
      (IsBefore(ICE->getBeginLoc(), ConditionSourceRange.getEnd()) ||
       !IsBefore(ConditionSourceRange.getEnd(), ICE->getBeginLoc())))
    if ((ICE->getCastKind() == clang::CastKind::CK_IntegralToBoolean) &&
        !ICE->isPartOfExplicitCast())
      if (!AD.GetSourceManager().isInSystemHeader(ICE->getBeginLoc())) {
        return !AD.reportWarning(ICE->getBeginLoc(),
                                 AutocheckWarnings::contiditionNotBool)
                    .limitReached();
      }

  return true;
}

void ConditionNotBoolVisitor::diagnoseCondition(clang::Expr *Condition) {
  if (Condition)
    ConditionSourceRange = Condition->getSourceRange();
}

/* Implementation of TypeWchartVisitor */

TypeWchartVisitor::TypeWchartVisitor(AutocheckDiagnostic &AD,
                                     clang::ASTContext &AC)
    : AD(AD), AC(AC) {}

bool TypeWchartVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::typeWchartUsed);
}

bool TypeWchartVisitor::VisitTypeLoc(const clang::TypeLoc &TL) {
  if (TL.getType()->isWideCharType() &&
      TL.getType().getDesugaredType(AC).getAsString() == "wchar_t") {
    return !AD.reportWarning(TL.getBeginLoc(),
                             AutocheckWarnings::typeWchartUsed)
                .limitReached();
  }
  return true;
}

/* Implementation of CStyleArrayVisitor */

CStyleArrayVisitor::CStyleArrayVisitor(AutocheckDiagnostic &AD,
                                       clang::ASTContext &AC)
    : AD(AD), AC(AC), IgnoredTypeLoc() {}

bool CStyleArrayVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::cStyleArrayUsed);
}

bool CStyleArrayVisitor::VisitVarDecl(const clang::VarDecl *VD) {
  if (VD->isConstexpr() && VD->isStaticDataMember()) {
    IgnoredTypeLoc = VD->getTypeSourceInfo()->getTypeLoc().getUnqualifiedLoc();
  }
  return true;
}

bool CStyleArrayVisitor::VisitCXXNewExpr(const clang::CXXNewExpr *NE) {
  if (NE->isArray()) {
    return !AD.reportWarning(
                  NE->getAllocatedTypeSourceInfo()->getTypeLoc().getBeginLoc(),
                  AutocheckWarnings::cStyleArrayUsed)
                .limitReached();
  }
  return true;
}

bool CStyleArrayVisitor::VisitTypeLoc(const clang::TypeLoc &TL) {
  // Skip Elaborated, Typedef and Using types to avoid getting multiple warnings
  // for same type.
  if (llvm::dyn_cast_if_present<clang::ElaboratedType>(TL.getTypePtr()) ||
      llvm::dyn_cast_if_present<clang::TypedefType>(TL.getTypePtr()) ||
      llvm::dyn_cast_if_present<clang::UsingType>(TL.getTypePtr()))
    return true;

  // Skip auto-deduced types.
  if (TL.getTypePtr()->getContainedAutoType())
    return true;

  if (IgnoredTypeLoc && IgnoredTypeLoc == TL) {
    return true;
  }

  if (const clang::ArrayType *AT =
          llvm::dyn_cast_if_present<clang::ArrayType>(TL.getType())) {
    // Warn only once in case of a nested array.
    if (llvm::dyn_cast_if_present<clang::ArrayType>(AT->getElementType()))
      return true;

    return !AD.reportWarning(TL.getBeginLoc(),
                             AutocheckWarnings::cStyleArrayUsed)
                .limitReached();
  }

  return true;
}

/* Implementation of BoolVectorUsedVisitor */

BoolVectorUsedVisitor::BoolVectorUsedVisitor(AutocheckDiagnostic &AD,
                                             clang::ASTContext &AC)
    : AD(AD), AC(AC) {}

bool BoolVectorUsedVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::boolVectorSpecializationUsed);
}

bool BoolVectorUsedVisitor::VisitTypeLoc(const clang::TypeLoc &TL) {
  // Skip Elaborated, Typedef and Using types to avoid getting multiple warnings
  // for same type.
  if (llvm::dyn_cast_if_present<clang::ElaboratedType>(TL.getTypePtr()) ||
      llvm::dyn_cast_if_present<clang::TypedefType>(TL.getTypePtr()) ||
      llvm::dyn_cast_if_present<clang::UsingType>(TL.getTypePtr()))
    return true;

  // Skip auto-deduced types.
  if (TL.getTypePtr()->getContainedAutoType())
    return true;

  const clang::TemplateSpecializationType *Type =
      llvm::dyn_cast_if_present<clang::TemplateSpecializationType>(
          TL.getTypePtr());
  if (Type &&
      TL.getType().getDesugaredType(AC).getAsString() ==
          "class std::vector<_Bool>" &&
      AD.GetSourceManager().isInSystemHeader(
          TL.getType()->getAsRecordDecl()->getLocation())) {
    return !AD.reportWarning(TL.getBeginLoc(),
                             AutocheckWarnings::boolVectorSpecializationUsed)
                .limitReached();
  }

  return true;
}

/* Implementation of TypeLongDoubleVisitor */

TypeLongDoubleVisitor::TypeLongDoubleVisitor(AutocheckDiagnostic &AD,
                                             clang::ASTContext &AC)
    : AD(AD), AC(AC) {}

bool TypeLongDoubleVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::longDoubleUsed);
}

bool TypeLongDoubleVisitor::VisitTypeLoc(const clang::TypeLoc &TL) {
  // Skip Elaborated, Typedef and Using types to avoid getting multiple warnings
  // for same type.
  if (llvm::dyn_cast_if_present<clang::ElaboratedType>(TL.getTypePtr()) ||
      llvm::dyn_cast_if_present<clang::TypedefType>(TL.getTypePtr()) ||
      llvm::dyn_cast_if_present<clang::UsingType>(TL.getTypePtr()))
    return true;

  // Skip auto-deduced types.
  if (TL.getTypePtr()->getContainedAutoType())
    return true;

  if (TL.getType()->isBuiltinType() && TL.getType()->isFloatingType() &&
      TL.getType().getDesugaredType(AC).getAsString() == "long double") {
    return !AD.reportWarning(TL.getBeginLoc(),
                             AutocheckWarnings::longDoubleUsed)
                .limitReached();
  }

  return true;
}

/* Implementation of AutoPtrVisitor */

AutoPtrVisitor::AutoPtrVisitor(AutocheckDiagnostic &AD, clang::ASTContext &AC)
    : AD(AD), AC(AC) {}

bool AutoPtrVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::typeAutoPtrUsed);
}

bool AutoPtrVisitor::VisitTypeLoc(const clang::TypeLoc &TL) {
  // Skip Elaborated, Typedef and Using types to avoid getting multiple warnings
  // for same type.
  if (llvm::dyn_cast_if_present<clang::ElaboratedType>(TL.getTypePtr()) ||
      llvm::dyn_cast_if_present<clang::TypedefType>(TL.getTypePtr()) ||
      llvm::dyn_cast_if_present<clang::UsingType>(TL.getTypePtr()))
    return true;

  // Skip auto-deduced types.
  if (TL.getTypePtr()->getContainedAutoType())
    return true;

  const clang::TemplateSpecializationType *Type =
      llvm::dyn_cast_if_present<clang::TemplateSpecializationType>(
          TL.getTypePtr());
  if (Type &&
      llvm::StringRef(TL.getType().getDesugaredType(AC).getAsString())
          .starts_with("class std::auto_ptr<") &&
      AD.GetSourceManager().isInSystemHeader(
          TL.getType()->getAsRecordDecl()->getLocation())) {
    return !AD.reportWarning(TL.getBeginLoc(),
                             AutocheckWarnings::typeAutoPtrUsed)
                .limitReached();
  }

  return true;
}

/* Implementation of DecltypeTypeidVisitor */

DecltypeTypeidVisitor::DecltypeTypeidVisitor(AutocheckDiagnostic &AD,
                                             clang::ASTContext &AC)
    : AD(AD), AC(AC) {}

bool DecltypeTypeidVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::lambdaDecltypeTypeid);
}

bool DecltypeTypeidVisitor::IsLambda(const clang::VarDecl *VD) const {
  if (VD == nullptr)
    return false;

  if (VD->hasInit())
    if (llvm::dyn_cast_if_present<clang::LambdaExpr>(VD->getInit()))
      return true;

  return false;
}

const clang::DeclRefExpr *
DecltypeTypeidVisitor::GetDRE(const clang::Expr *E) const {
  if (const clang::DeclRefExpr *DRE =
          llvm::dyn_cast_if_present<clang::DeclRefExpr>(E))
    return DRE;

  if (const clang::ImplicitCastExpr *ICE =
          llvm::dyn_cast_if_present<clang::ImplicitCastExpr>(E))
    if (const clang::DeclRefExpr *DRE =
            llvm::dyn_cast_if_present<clang::DeclRefExpr>(ICE->getSubExpr()))
      return DRE;

  return nullptr;
}

const clang::VarDecl *
DecltypeTypeidVisitor::GetVarDecl(const clang::DeclRefExpr *DRE) const {
  if (DRE == nullptr)
    return nullptr;

  if (const clang::VarDecl *VarD =
          llvm::dyn_cast_if_present<clang::VarDecl>(DRE->getDecl()))
    return VarD;

  return nullptr;
}

bool DecltypeTypeidVisitor::VisitTypeLoc(const clang::TypeLoc &TL) {
  if (const clang::DecltypeType *DTT =
          llvm::dyn_cast_if_present<clang::DecltypeType>(TL.getTypePtr()))
    if (IsLambda(GetVarDecl(GetDRE(DTT->getUnderlyingExpr()))))
      if (AD.reportWarning(DTT->getUnderlyingExpr()->getBeginLoc(),
                           AutocheckWarnings::lambdaDecltypeTypeid)
              .limitReached())
        return false;
  return true;
}

bool DecltypeTypeidVisitor::VisitCXXTypeidExpr(
    const clang::CXXTypeidExpr *CTE) {
  if (IsLambda(GetVarDecl(GetDRE(CTE->getExprOperand())))) {
    if (AD.reportWarning(CTE->getExprOperand()->getBeginLoc(),
                         AutocheckWarnings::lambdaDecltypeTypeid)
            .limitReached())
      return false;
  }
  return true;
}

/* Implementation of TypesVisitor */

TypesVisitor::TypesVisitor(AutocheckDiagnostic &AD, clang::ASTContext &AC)
    : AD(AD) {
  const AutocheckContext &Context = AD.GetContext();
  if (CharStorageVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<CharStorageVisitor>(AD, AC));
  if (SignCharStorageVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<SignCharStorageVisitor>(AD, AC));
  if (ConditionNotBoolVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<ConditionNotBoolVisitor>(AD));
  if (TypeWchartVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<TypeWchartVisitor>(AD, AC));
  if (BoolVectorUsedVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<BoolVectorUsedVisitor>(AD, AC));
  if (CStyleArrayVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<CStyleArrayVisitor>(AD, AC));
  if (TypeLongDoubleVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<TypeLongDoubleVisitor>(AD, AC));
  if (AutoPtrVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<AutoPtrVisitor>(AD, AC));
  if (DecltypeTypeidVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<DecltypeTypeidVisitor>(AD, AC));
}

void TypesVisitor::run(clang::TranslationUnitDecl *TUD) {
  if (!AllVisitors.empty())
    this->TraverseDecl(TUD);
}

bool TypesVisitor::TraverseDecl(clang::Decl *D) {
  if (!D) // Can be null if code had errors.
    return true;

  clang::SourceLocation Loc = D->getBeginLoc();

  if (Loc.isInvalid() || appropriateHeaderLocation(AD, Loc)) {
    clang::RecursiveASTVisitor<TypesVisitor>::TraverseDecl(D);
  }
  return true;
}

bool TypesVisitor::VisitIfStmt(clang::IfStmt *IS) {
  AllVisitors.remove_if([IS](std::unique_ptr<TypesVisitorInterface> &V) {
    return !V->VisitIfStmt(IS);
  });
  return true;
}

bool TypesVisitor::VisitWhileStmt(clang::WhileStmt *WS) {
  AllVisitors.remove_if([WS](std::unique_ptr<TypesVisitorInterface> &V) {
    return !V->VisitWhileStmt(WS);
  });
  return true;
}

bool TypesVisitor::VisitForStmt(clang::ForStmt *FS) {
  AllVisitors.remove_if([FS](std::unique_ptr<TypesVisitorInterface> &V) {
    return !V->VisitForStmt(FS);
  });
  return true;
}

bool TypesVisitor::VisitDoStmt(clang::DoStmt *DS) {
  AllVisitors.remove_if([DS](std::unique_ptr<TypesVisitorInterface> &V) {
    return !V->VisitDoStmt(DS);
  });
  return true;
}

bool TypesVisitor::VisitImplicitCastExpr(const clang::ImplicitCastExpr *ICE) {
  AllVisitors.remove_if([ICE](std::unique_ptr<TypesVisitorInterface> &V) {
    return !V->VisitImplicitCastExpr(ICE);
  });
  return true;
}

bool TypesVisitor::VisitTypeLoc(const clang::TypeLoc &TL) {
  AllVisitors.remove_if([TL](std::unique_ptr<TypesVisitorInterface> &V) {
    return !V->VisitTypeLoc(TL);
  });
  return true;
}

bool TypesVisitor::VisitVarDecl(const clang::VarDecl *VD) {
  AllVisitors.remove_if([VD](std::unique_ptr<TypesVisitorInterface> &V) {
    return !V->VisitVarDecl(VD);
  });
  return true;
}

bool TypesVisitor::VisitCXXNewExpr(const clang::CXXNewExpr *NE) {
  AllVisitors.remove_if([NE](std::unique_ptr<TypesVisitorInterface> &V) {
    return !V->VisitCXXNewExpr(NE);
  });
  return true;
}

bool TypesVisitor::VisitCXXTypeidExpr(const clang::CXXTypeidExpr *CTE) {
  AllVisitors.remove_if([CTE](std::unique_ptr<TypesVisitorInterface> &V) {
    return !V->VisitCXXTypeidExpr(CTE);
  });
  return true;
}

} // namespace autocheck
