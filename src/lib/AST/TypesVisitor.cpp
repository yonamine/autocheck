//===--- TypesVisitor.cpp - Visitor for type related checks ---------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
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

/* Implementation of CharStorageVisitor */

CharStorageVisitor::CharStorageVisitor(clang::DiagnosticsEngine &DE,
                                       clang::ASTContext &ASTCtx)
    : DE(DE), ASTCtx(ASTCtx) {}

bool CharStorageVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::charStorage);
}

bool CharStorageVisitor::VisitImplicitCastExpr(
    const clang::ImplicitCastExpr *E) {
  if (E->getCastKind() == clang::CK_IntegralCast && isPlainChar(E->getType())) {
    return !AutocheckDiagnostic::reportWarning(DE, E->getBeginLoc(),
                                               AutocheckWarnings::charStorage,
                                               E->getSourceRange())
                .limitReached();
  }
  return true;
}

bool CharStorageVisitor::isPlainChar(const clang::QualType &Type) const {
  return (Type->isCharType() &&
          Type.getDesugaredType(ASTCtx).getAsString() == "char");
}

/* Implementation of SignCharStorageVisitor */

SignCharStorageVisitor::SignCharStorageVisitor(clang::DiagnosticsEngine &DE,
                                               clang::ASTContext &ASTCtx)
    : DE(DE), ASTCtx(ASTCtx) {}

bool SignCharStorageVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::signCharStorage);
}

bool SignCharStorageVisitor::VisitImplicitCastExpr(
    const clang::ImplicitCastExpr *ICE) {
  if (ICE->getCastKind() == clang::CK_IntegralCast &&
      isUnsignedOrSignedChar(ICE->getType()) &&
      isPlainChar(ICE->getSubExpr()->getType())) {
    return !AutocheckDiagnostic::reportWarning(
                DE, ICE->getBeginLoc(), AutocheckWarnings::signCharStorage,
                ICE->getSourceRange())
                .limitReached();
  }

  return true;
}

bool SignCharStorageVisitor::isPlainChar(const clang::QualType &Type) const {
  return (Type->isCharType() &&
          Type.getDesugaredType(ASTCtx).getAsString() == "char");
}

bool SignCharStorageVisitor::isUnsignedOrSignedChar(
    const clang::QualType &Type) {
  if (Type->isCharType()) {
    const std::string TypeName = Type.getDesugaredType(ASTCtx).getAsString();
    return (TypeName == "unsigned char" || TypeName == "signed char");
  }
  return false;
}

/* Implementation of ConditionNotBoolVisitor */

ConditionNotBoolVisitor::ConditionNotBoolVisitor(clang::DiagnosticsEngine &DE)
    : DE(DE), IsBefore(DE.getSourceManager()) {}

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
      if (!DE.getSourceManager().isInSystemHeader(ICE->getBeginLoc())) {
        return !AutocheckDiagnostic::reportWarning(
                    DE, ICE->getBeginLoc(),
                    AutocheckWarnings::contiditionNotBool)
                    .limitReached();
      }

  return true;
}

void ConditionNotBoolVisitor::diagnoseCondition(clang::Expr *Condition) {
  if (Condition)
    ConditionSourceRange = Condition->getSourceRange();
}

/* Implementation of TypesVisitor */

TypesVisitor::TypesVisitor(clang::DiagnosticsEngine &DE, clang::ASTContext &AC)
    : DE(DE) {
  const AutocheckContext &Context = AutocheckContext::Get();
  if (CharStorageVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<CharStorageVisitor>(DE, AC));
  if (SignCharStorageVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<SignCharStorageVisitor>(DE, AC));
  if (ConditionNotBoolVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<ConditionNotBoolVisitor>(DE));
}

void TypesVisitor::run(clang::TranslationUnitDecl *TUD) {
  if (!AllVisitors.empty())
    this->TraverseDecl(TUD);
}

bool TypesVisitor::TraverseDecl(clang::Decl *D) {
  if (!D) // Can be null if code had errors.
    return true;

  clang::SourceLocation Loc = D->getBeginLoc();

  if (Loc.isInvalid() || appropriateHeaderLocation(DE, Loc)) {
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

} // namespace autocheck
