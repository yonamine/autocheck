//===--- ClassesVisitor.cpp - Visitor for class related checks ------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the ClassesVisitor class and all classes derived from
// ClassesVisitorInterface.
//
//===----------------------------------------------------------------------===//

#include "AST/ClassesVisitor.h"

#include "AST/ASTHelper.h"
#include "Diagnostics/AutocheckDiagnostic.h"
#include "clang/Basic/DiagnosticSema.h"

namespace autocheck {

static bool isMethodMarkedOverride(const clang::Decl *D) {
  for (const auto &A : D->attrs())
    if (A->getKind() == clang::attr::Kind::Override)
      return true;
  return false;
}

static bool isMethodOrClassFinal(const clang::Decl *D) {
  for (const auto &A : D->attrs())
    if (A->getKind() == clang::attr::Kind::Final)
      return true;
  return false;
}

/* ClassesVisitorInterface */

ClassesVisitorInterface::~ClassesVisitorInterface() {}

using CVI = ClassesVisitorInterface;
void CVI::PostWork() {}
bool CVI::PreTraverseDecl(clang::Decl *) { return true; }
bool CVI::PostTraverseDecl(clang::Decl *) { return true; }
bool CVI::VisitCXXRecordDecl(const clang::CXXRecordDecl *) { return true; }
bool CVI::VisitCXXMethodDecl(const clang::CXXMethodDecl *) { return true; }
bool CVI::VisitCXXConversionDecl(const clang::CXXConversionDecl *) {
  return true;
}
bool CVI::VisitCXXMemberCallExpr(const clang::CXXMemberCallExpr *) {
  return true;
}
bool CVI::VisitFunctionDecl(const clang::FunctionDecl *) { return true; }

/* DerivedFromVirtualVisitor */

DerivedFromVirtualVisitor::DerivedFromVirtualVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool DerivedFromVirtualVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::derivedFromVirtual);
}

bool DerivedFromVirtualVisitor::VisitCXXRecordDecl(
    const clang::CXXRecordDecl *CRD) {
  if (CRD->getDefinition()) {
    // Get all virtual base classes and report a warning message for each.
    const auto &End = CRD->vbases_end();
    for (auto It = CRD->vbases_begin(); It != End; ++It) {
      if (AutocheckDiagnostic::reportWarning(
              DE, CRD->getBeginLoc(), AutocheckWarnings::derivedFromVirtual)
              .limitReached())
        return false;
    }
  }

  return true;
}

/* VirtualFuncSpecifierVisitor */

VirtualFuncSpecifierVisitor::VirtualFuncSpecifierVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool VirtualFuncSpecifierVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::virtualFuncSpecifier);
}

bool VirtualFuncSpecifierVisitor::VisitCXXMethodDecl(
    const clang::CXXMethodDecl *CMD) {
  // Check if the function is virtual.
  if (CMD->isVirtual()) {
    const bool IsMarkedVirtual = CMD->isVirtualAsWritten();
    const bool IsMarkedFinal = isMethodOrClassFinal(CMD);
    const bool IsMarkedOverride = isMethodMarkedOverride(CMD);
    if ((IsMarkedVirtual && (IsMarkedFinal || IsMarkedOverride)) ||
        (IsMarkedFinal && (IsMarkedVirtual || IsMarkedOverride)) ||
        (IsMarkedOverride && (IsMarkedVirtual || IsMarkedFinal)) ||
        !(IsMarkedOverride || IsMarkedVirtual || IsMarkedFinal)) {
      return !AutocheckDiagnostic::reportWarning(
                  DE, CMD->getBeginLoc(),
                  AutocheckWarnings::virtualFuncSpecifier)
                  .limitReached();
    }
  }

  return true;
}

/* OverrideFuncSpecifierVisitor */

OverrideFuncSpecifierVisitor::OverrideFuncSpecifierVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool OverrideFuncSpecifierVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::overrideFuncSpecifier);
}

bool OverrideFuncSpecifierVisitor::VisitCXXMethodDecl(
    const clang::CXXMethodDecl *CMD) {
  // Check if the function is virtual.
  if (CMD->isVirtual()) {
    const bool IsMarkedFinal = isMethodOrClassFinal(CMD);
    const bool IsMarkedOverride = isMethodMarkedOverride(CMD);

    if (CMD->size_overridden_methods() &&
        ((IsMarkedFinal && IsMarkedOverride) ||
         !(IsMarkedFinal || IsMarkedOverride))) {
      return !AutocheckDiagnostic::reportWarning(
                  DE, CMD->getBeginLoc(),
                  AutocheckWarnings::overrideFuncSpecifier)
                  .limitReached();
    }
  }

  return true;
}

/* VirtualMethodFinalClassVisitor */

VirtualMethodFinalClassVisitor::VirtualMethodFinalClassVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool VirtualMethodFinalClassVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::virtualMethodFinalClass);
}

bool VirtualMethodFinalClassVisitor::VisitCXXRecordDecl(
    const clang::CXXRecordDecl *CRD) {
  if (CRD->getDefinition()) {
    if (isMethodOrClassFinal(CRD))
      for (const auto &M : CRD->methods())
        if (M->isUserProvided() && M->isVirtual() && !isMethodOrClassFinal(M)) {
          if (AutocheckDiagnostic::reportWarning(
                  DE, M->getBeginLoc(),
                  AutocheckWarnings::virtualMethodFinalClass)
                  .limitReached())
            return false;
        }
  }

  return true;
}

/* PureFuncOverrideVisitor */

PureFuncOverrideVisitor::PureFuncOverrideVisitor(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool PureFuncOverrideVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::pureFuncOverride);
}

bool PureFuncOverrideVisitor::VisitCXXMethodDecl(
    const clang::CXXMethodDecl *CMD) {
  if (CMD->isPure())
    for (const auto &M : CMD->overridden_methods())
      if (!M->isPure()) {
        if (AutocheckDiagnostic::reportWarning(
                DE, CMD->getBeginLoc(), AutocheckWarnings::pureFuncOverride)
                .limitReached())
          return false;
      }
  return true;
}

/* MemberDataPrivateVisitor */

MemberDataPrivateVisitor::MemberDataPrivateVisitor(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool MemberDataPrivateVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::memberDataPrivate);
}

bool MemberDataPrivateVisitor::VisitCXXRecordDecl(
    const clang::CXXRecordDecl *CRD) {
  if (CRD->getDefinition()) {
    if (!CRD->isPOD()) {
      const auto &Fields = CRD->fields();
      for (const auto &F : Fields)
        if (F->getAccess() != clang::AccessSpecifier::AS_private) {
          if (AutocheckDiagnostic::reportWarning(
                  DE, F->getBeginLoc(), AutocheckWarnings::memberDataPrivate)
                  .limitReached())
            return false;
        }
    }
  }

  return true;
}

/* NonPODAsClassVisitor */

NonPODAsClassVisitor::NonPODAsClassVisitor(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool NonPODAsClassVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::nonPodAsClass);
}

bool NonPODAsClassVisitor::VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) {
  if (CRD->getDefinition()) {
    if (!CRD->isPOD() && !CRD->isClass()) {
      return !AutocheckDiagnostic::reportWarning(
                  DE, CRD->getBeginLoc(), AutocheckWarnings::nonPodAsClass)
                  .limitReached();
    }
  }

  return true;
}

/* ExplicitCtorsVisitor */

ExplicitCtorsVisitor::ExplicitCtorsVisitor(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool ExplicitCtorsVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::explicitCtors);
}

bool ExplicitCtorsVisitor::VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) {
  if (CRD->getDefinition()) {
    for (const auto &C : CRD->ctors()) {
      if (!C->isExplicit()) {
        if (C->getMinRequiredArguments() > 1)
          continue;
        if (C->getMinRequiredArguments() == 1) {
          const auto P = C->parameters()[0];
          if (P->getType()->isFundamentalType()) {
            return !AutocheckDiagnostic::reportWarning(
                        DE, CRD->getBeginLoc(),
                        AutocheckWarnings::explicitCtors)
                        .limitReached();
          }
        }
      }
    }
  }

  return true;
}

/* FinalClassDestructorVisitor */

FinalClassDestructorVisitor::FinalClassDestructorVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool FinalClassDestructorVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::finalClassDestructor);
}

bool FinalClassDestructorVisitor::VisitCXXRecordDecl(
    const clang::CXXRecordDecl *CRD) {

  if (CRD->isImplicit() || !CRD->isClass() ||
      !CRD->isThisDeclarationADefinition())
    return true;

  if (isMethodOrClassFinal(CRD))
    return true;

  if (CRD->getDefinition()) {
    if (CRD->hasSimpleDestructor()) {
      return !AutocheckDiagnostic::reportWarning(
                  DE, CRD->getBeginLoc(),
                  AutocheckWarnings::finalClassDestructor)
                  .limitReached();
    }
    if (const clang::CXXDestructorDecl *CDD = CRD->getDestructor()) {
      if ((CDD->getAccess() == clang::AccessSpecifier::AS_public) &&
          !CDD->isVirtual()) {
        return !AutocheckDiagnostic::reportWarning(
                    DE, CRD->getBeginLoc(),
                    AutocheckWarnings::finalClassDestructor)
                    .limitReached();
      }
    }
  }

  return true;
}

/* RelOpBoolValueVisitor */

RelOpBoolValueVisitor::RelOpBoolValueVisitor(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool RelOpBoolValueVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::relOpBoolValue);
}

bool RelOpBoolValueVisitor::VisitCXXMethodDecl(
    const clang::CXXMethodDecl *CMD) {
  const clang::OverloadedOperatorKind OOK = CMD->getOverloadedOperator();
  switch (OOK) {
  case clang::OverloadedOperatorKind::OO_Less:
  case clang::OverloadedOperatorKind::OO_Greater:
  case clang::OverloadedOperatorKind::OO_LessEqual:
  case clang::OverloadedOperatorKind::OO_GreaterEqual:
  case clang::OverloadedOperatorKind::OO_EqualEqual:
  case clang::OverloadedOperatorKind::OO_ExclaimEqual:
    if (!CMD->getReturnType()->isBooleanType()) {
      return !AutocheckDiagnostic::reportWarning(
                  DE, CMD->getBeginLoc(), AutocheckWarnings::relOpBoolValue)
                  .limitReached();
    }
    break;
  default:
    break;
  }

  return true;
}

/* ImplicitConversionOpVisitor */

ImplicitConversionOpVisitor::ImplicitConversionOpVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool ImplicitConversionOpVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::implicitConversionOp);
}

bool ImplicitConversionOpVisitor::VisitCXXConversionDecl(
    const clang::CXXConversionDecl *CCD) {
  if (!CCD->isExplicit()) {
    return !AutocheckDiagnostic::reportWarning(
                DE, CCD->getBeginLoc(), AutocheckWarnings::implicitConversionOp)
                .limitReached();
  }
  return true;
}

/* ConverionOpUsedVisitor */

ConverionOpUsedVisitor::ConverionOpUsedVisitor(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool ConverionOpUsedVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::conversionOpUsed);
}

bool ConverionOpUsedVisitor::VisitCXXConversionDecl(
    const clang::CXXConversionDecl *CCD) {
  return !AutocheckDiagnostic::reportWarning(
              DE, CCD->getBeginLoc(), AutocheckWarnings::conversionOpUsed)
              .limitReached();
}

/* SubscriptOperatorOverloadVisitor */

SubscriptOperatorOverloadVisitor::SubscriptOperatorOverloadVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool SubscriptOperatorOverloadVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::subscriptOpOverload);
}

bool SubscriptOperatorOverloadVisitor::VisitCXXRecordDecl(
    const clang::CXXRecordDecl *CRD) {
  const clang::CXXMethodDecl *NonConst = nullptr;
  for (const clang::CXXMethodDecl *CMD : CRD->methods()) {
    if (CMD->getOverloadedOperator() ==
        clang::OverloadedOperatorKind::OO_Subscript) {
      if (CMD->isConst())
        return true; // Has const version, this CXXRecord is compliant.
      else
        NonConst = CMD;
    }
  }
  if (NonConst)
    return !AutocheckDiagnostic::reportWarning(
                DE, NonConst->getBeginLoc(),
                AutocheckWarnings::subscriptOpOverload)
                .limitReached();
  return true;
}

/* UnusedPrivateMethodVisitor */

UnusedPrivateMethodVisitor::UnusedPrivateMethodVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool UnusedPrivateMethodVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::unusedFunctionOrMethod);
}

void UnusedPrivateMethodVisitor::PostWork() {
  for (const clang::CXXMethodDecl *CMD : PrivateMethods) {
    if (AutocheckDiagnostic::reportWarning(
            DE, CMD->getLocation(), AutocheckWarnings::unusedFunctionOrMethod)
            .limitReached())
      break;
  }
}

bool UnusedPrivateMethodVisitor::VisitCXXRecordDecl(
    const clang::CXXRecordDecl *CRD) {
  if (CRD->isThisDeclarationADefinition())
    for (const clang::CXXMethodDecl *CMD : CRD->methods())
      if (CMD->getAccess() == clang::AccessSpecifier::AS_private)
        PrivateMethods.insert(CMD);
  return true;
}

bool UnusedPrivateMethodVisitor::VisitCXXMemberCallExpr(
    const clang::CXXMemberCallExpr *CMCE) {
  const clang::CXXMethodDecl *CMD = CMCE->getMethodDecl();
  if (!CMD)
    return true;
  if (CMD->getAccess() == clang::AccessSpecifier::AS_private)
    PrivateMethods.erase(CMD);
  return true;
}

/* NonConstReturnedFromConst */

bool isStandardSmartPointer(const clang::Type *TheType) {
  if (!TheType)
    return false;
  const clang::CXXRecordDecl *RecordDecl = TheType->getAsCXXRecordDecl();
  if (!RecordDecl)
    return false;
  const clang::IdentifierInfo *ID = RecordDecl->getIdentifier();
  if (!ID)
    return false;
  llvm::StringRef Name = ID->getName();
  if (Name != "unique_ptr" && Name != "shared_ptr" && Name != "weak_ptr")
    return false;
  return RecordDecl->getDeclContext()->isStdNamespace();
}

clang::QualType getBasicType(clang::QualType T) {
  while (T->isPointerType() || T->isReferenceType())
    T = T->isPointerType() ? T->getPointeeType() : T.getNonReferenceType();
  return T;
}

std::set<const clang::Type *>
FindAllClassDataBasicTypes(const clang::CXXRecordDecl *CRD) {
  std::set<const clang::Type *> FieldTypes;
  // Find basic types of class-data (for example, int32_t is a basic type
  // for int32_t ***).
  for (const clang::FieldDecl *FD : CRD->fields()) {
    clang::QualType QT = getBasicType(FD->getType());
    if (QT->isFundamentalType())
      FieldTypes.insert(QT.getTypePtr());
    else // Important for smart pointers.
      FieldTypes.insert(QT->getUnqualifiedDesugaredType());
  }

  // Find basic types of not private class-data inherited from the base
  // classes.
  for (const clang::CXXBaseSpecifier &BS : CRD->bases()) {
    const clang::CXXRecordDecl *Base = BS.getType()->getAsCXXRecordDecl();
    if (!Base)
      continue;
    for (const clang::FieldDecl *FD : Base->fields())
      if (FD->getAccess() != clang::AccessSpecifier::AS_private) {
        clang::QualType QT = getBasicType(FD->getType());
        if (QT->isFundamentalType())
          FieldTypes.insert(QT.getTypePtr());
        else // Important for smart pointers.
          FieldTypes.insert(QT->getUnqualifiedDesugaredType());
      }
  }
  return FieldTypes;
}

bool CheckReturnType(std::set<const clang::Type *> FieldTypes,
                     clang::QualType T) {
  if ((T->isPointerType() && !T->getPointeeType().isConstQualified()) ||
      (T->isReferenceType() && !T.getNonReferenceType().isConstQualified()) ||
      (isStandardSmartPointer(T.getTypePtrOrNull()) && !T.isConstQualified())) {
    T = getBasicType(T);
    auto search = T->isFundamentalType()
                      ? FieldTypes.find(T.getTypePtr())
                      : FieldTypes.find(T->getUnqualifiedDesugaredType());
    if (search != FieldTypes.end())
      return true;
  }
  return false;
}

NonConstReturnedFromConst::NonConstReturnedFromConst(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool NonConstReturnedFromConst::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::nonConstReturnedFromConst);
}

bool NonConstReturnedFromConst::VisitCXXRecordDecl(
    const clang::CXXRecordDecl *CRD) {
  if (!CRD->isThisDeclarationADefinition())
    return true;

  // Find set of basic types of all class-data.
  std::set<const clang::Type *> ClassDataTypes =
      FindAllClassDataBasicTypes(CRD);

  for (const clang::CXXMethodDecl *CMD : CRD->methods()) {
    if (!CMD->isConst())
      continue;
    clang::QualType T = CMD->getReturnType();
    // If return type is a pointer, reference or smart pointer which basic type
    // is in the set of the basic types of all class-data, than report warning -
    // otherwise, don't. This is a heuristic to avoid false positives.
    if (CheckReturnType(ClassDataTypes, T)) {
      if (AutocheckDiagnostic::reportWarning(
              DE, CMD->getBeginLoc(),
              AutocheckWarnings::nonConstReturnedFromConst)
              .limitReached())
        return false;
    }
  }
  return true;
}

/* CopyAndMoveAssignmentNoSwapVisitor */

static bool hasNoexceptSpecifier(const clang::FunctionDecl *FD) {
  const clang::FunctionProtoType *Type =
      FD->getType()->getAs<clang::FunctionProtoType>();
  const clang::ExceptionSpecificationType EST = Type->getExceptionSpecType();

  if (EST != clang::EST_None && EST != clang::EST_NoexceptFalse &&
      EST != clang::EST_Dynamic)
    return true;
  return false;
}

static bool hasNoexceptSwapFunctionCall(const clang::Stmt *Body) {
  // Iterative postorder tree traversal using two stacks technique.
  std::stack<const clang::Stmt *> PostOrderTraversalStack;
  std::stack<const clang::Stmt *> TmpStack;

  TmpStack.push(Body);

  while (!TmpStack.empty()) {

    auto *TmpStackStackTop = TmpStack.top();
    TmpStack.pop();

    if (!TmpStackStackTop)
      continue;

    PostOrderTraversalStack.push(TmpStackStackTop);

    for (auto *child : TmpStackStackTop->children())
      TmpStack.push(child);
  }

  while (!PostOrderTraversalStack.empty()) {
    auto *child = PostOrderTraversalStack.top();
    PostOrderTraversalStack.pop();

    if (!child)
      continue;

    if (const clang::CallExpr *CE =
            llvm::dyn_cast_if_present<clang::CallExpr>(child)) {
      const clang::Decl *D = CE->getCalleeDecl();
      if (const clang::FunctionDecl *FD =
              llvm::dyn_cast_if_present<clang::FunctionDecl>(D)) {
        if (isSwapFunction(FD) && hasNoexceptSpecifier(FD))
          return true;
      }
    }
  }
  return false;
}

CopyAndMoveAssignmentNoSwapVisitor::CopyAndMoveAssignmentNoSwapVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool CopyAndMoveAssignmentNoSwapVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::swapNotInCopyAndMoveAssignment);
}

bool CopyAndMoveAssignmentNoSwapVisitor::VisitCXXMethodDecl(
    const clang::CXXMethodDecl *CMD) {
  if (!CMD->isCopyAssignmentOperator() && !CMD->isMoveAssignmentOperator())
    return true;

  if (!CMD->isUserProvided())
    return true;

  const clang::FunctionDecl *Definition = CMD->getDefinition();
  if (!Definition)
    return true;

  const clang::Stmt *Body = Definition->getBody();

  if (!hasNoexceptSwapFunctionCall(Body))
    return !AutocheckDiagnostic::reportWarning(
                DE, CMD->getBeginLoc(),
                AutocheckWarnings::swapNotInCopyAndMoveAssignment)
                .limitReached();
  return true;
}

/* UnionsUsedVisitor */

UnionsUsedVisitor::UnionsUsedVisitor(clang::DiagnosticsEngine &DE,
                                     clang::ASTContext &AC)
    : DE(DE), AC(AC) {}

bool UnionsUsedVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::unionsUsed);
}

bool UnionsUsedVisitor::VisitCXXRecordDecl(const clang::CXXRecordDecl *D) {
  if (D->isUnion()) {
    // Check if union is a tagged union. Tagged union is a data structure
    // (defined as struct data type) used to hold a value that could take on
    // several different types (defined as union type), and has a hidden tag
    // field (defined with enum type) explicitly indicates which one is in
    // use. A union is a tagged union if it is defined within a struct data
    // type together with an enum that has the same number of fields as it
    // does.
    bool IsTaggedUnion = false;

    // In the C++ Standard Library (C++17) tagged unions are not allowed
    // since std::variant is available.
    if (AC.getLangOpts().CPlusPlus17) {
      return !AutocheckDiagnostic::reportWarning(DE, D->getBeginLoc(),
                                                 AutocheckWarnings::unionsUsed)
                  .limitReached();
    }
    const clang::CXXRecordDecl *RD =
        llvm::dyn_cast_if_present<clang::CXXRecordDecl>(D->getParent());
    if (!RD || !RD->isStruct()) {
      return !AutocheckDiagnostic::reportWarning(DE, D->getBeginLoc(),
                                                 AutocheckWarnings::unionsUsed)
                  .limitReached();
    }
    int NumEnumDecl = 0;
    const clang::EnumDecl *ED;
    for (const clang::Decl *Dcl : RD->decls())
      if (const clang::EnumDecl *EDcl = llvm::dyn_cast<clang::EnumDecl>(Dcl)) {
        NumEnumDecl++;
        ED = EDcl;
        if (NumEnumDecl > 1)
          break;
      }
    if (NumEnumDecl == 1) {
      unsigned NumEnumFields =
          std::distance(ED->enumerator_begin(), ED->enumerator_end());
      unsigned NumUnionFields = std::distance(D->field_begin(), D->field_end());
      if (NumEnumFields == NumUnionFields)
        IsTaggedUnion = true;
    }
    if (!IsTaggedUnion) {
      return !AutocheckDiagnostic::reportWarning(DE, D->getBeginLoc(),
                                                 AutocheckWarnings::unionsUsed)
                  .limitReached();
    }
  }
  return true;
}

/* ChangedDefaultArgumentsVisitor */

bool ChangedDefaultArgumentsVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::changedDefaultArguments);
}

void ChangedDefaultArgumentsVisitor::evaluteResults(
    const clang::Expr *OverridenDAE, const clang::Expr *OverridingDAE,
    clang::Expr::EvalResult &ResultOfOverridenExpr,
    clang::Expr::EvalResult &ResultOfOverridingExpr) {

  if (OverridenDAE->isLValue()) {
    OverridenDAE->EvaluateAsLValue(ResultOfOverridenExpr, ASTCtx, true);
  } else if (OverridenDAE->isPRValue()) {
    OverridenDAE->EvaluateAsRValue(ResultOfOverridenExpr, ASTCtx, true);
  }

  if (OverridingDAE->isLValue()) {
    OverridingDAE->EvaluateAsLValue(ResultOfOverridingExpr, ASTCtx, false);
  } else if (OverridingDAE->isPRValue()) {
    OverridingDAE->EvaluateAsRValue(ResultOfOverridingExpr, ASTCtx, true);
  }
}

bool ChangedDefaultArgumentsVisitor::compareResults(
    clang::Expr::EvalResult &ResultOfOverridenExpr,
    clang::Expr::EvalResult &ResultOfOverridingExpr, clang::QualType Ty) {
  return ResultOfOverridenExpr.Val.getAsString(ASTCtx, Ty) ==
         ResultOfOverridingExpr.Val.getAsString(ASTCtx, Ty);
}

const clang::CXXConstructExpr *
ChangedDefaultArgumentsVisitor::getCXXConstructExpr(const clang::Expr *E) {
  const clang::Stmt *Child = static_cast<const clang::Stmt *>(E);
  do {
    if (const clang::CXXConstructExpr *CE =
            llvm::dyn_cast_if_present<const clang::CXXConstructExpr>(Child)) {
      // Default arguments are constructed using move initialization. For
      // example: A a = A{1,2}. First, temporary object is constructed with
      // A{1,2} and then it is moved into variable 'a' with move constructor. To
      // get construction arguments of an object, we need
      // clang::CXXConstructExpr, but not the move constructor.
      if (!CE->getConstructor()->isMoveConstructor())
        return CE;
    }
  } while (
      (Child = ((Child->child_begin() == nullptr) ? nullptr
                                                  : *(Child->child_begin()))));

  return nullptr;
}

void ChangedDefaultArgumentsVisitor::handleEvaluatableArguments(
    const clang::Expr *OverridenDAE, const clang::Expr *OverridingDAE,
    const clang::SourceLocation &Loc) {
  evaluteResults(OverridenDAE, OverridingDAE, ResultOfOverridenExpr,
                 ResultOfOverridingExpr);

  if (!compareResults(ResultOfOverridenExpr, ResultOfOverridingExpr,
                      OverridenDAE->getType()))
    Locations.insert(Loc);
}

void ChangedDefaultArgumentsVisitor::handleUnevaluatableArguments(
    const clang::Expr *OverridenDAE, const clang::Expr *OverridingDAE,
    const clang::SourceLocation &Loc) {
  // If expression can't be evaluated and is not record type, ignore it.
  // It's enough to check type for one expression, because code where this two
  // expressions have different types won't compile.
  if (!OverridingDAE->getType()->isRecordType())
    return;

  // Obtain construction expressions for objects in default arguments.
  // Variable name suffix DAE stands for Default Argument Expression.
  const clang::CXXConstructExpr *CCOverridenDAE =
      getCXXConstructExpr(OverridenDAE);
  const clang::CXXConstructExpr *CCOverridingDAE =
      getCXXConstructExpr(OverridingDAE);

  // If CXXConstructionExpr couldn't be found, leave.
  if (!CCOverridenDAE || !CCOverridingDAE)
    return;

  // Warn if default argument expressions use different constructors to create
  // an object.
  if (CCOverridenDAE->getConstructor() != CCOverridingDAE->getConstructor()) {
    Locations.insert(Loc);
    return;
  }
  // If constructors are the same, check if their arguments are as well.
  // Variable name suffix DAEC stands for Default Argument Expression
  // Constructor.
  auto ArgOfOverridingDAEC = CCOverridingDAE->arg_begin();
  for (auto ArgOfOverridenDAEC = CCOverridenDAE->arg_begin();
       ArgOfOverridenDAEC != CCOverridenDAE->arg_end();
       ArgOfOverridenDAEC++, ArgOfOverridingDAEC++) {

    // If they are evaluatable, compare their results, if not, skip this
    // argument.
    if ((*ArgOfOverridingDAEC)->isEvaluatable(ASTCtx) &&
        (*ArgOfOverridenDAEC)->isEvaluatable(ASTCtx)) {
      handleEvaluatableArguments(*ArgOfOverridingDAEC, *ArgOfOverridenDAEC,
                                 Loc);
    }
  }
}

void ChangedDefaultArgumentsVisitor::checkParameters(
    clang::ParmVarDecl *OverridenParamVarDecl,
    clang::ParmVarDecl *OverridingParamVarDecl) {
  const clang::SourceLocation Loc = OverridingParamVarDecl->getBeginLoc();

  // If overriding method doesn't have default argument, it is compliant no
  // matter whether overriden method has default argument or not.
  if (!OverridingParamVarDecl->hasDefaultArg())
    return;

  // If overridden method doesn't have default argument and overriding method
  // has, rule is violated.
  if (!OverridenParamVarDecl->hasDefaultArg()) {
    Locations.insert(Loc);
    return;
  }

  // Case where both methods have default argument.
  clang::Expr *OverridenDAE = OverridingParamVarDecl->getDefaultArg();
  clang::Expr *OverridingDAE =
      OverridenParamVarDecl->hasUninstantiatedDefaultArg()
          ? OverridenParamVarDecl->getUninstantiatedDefaultArg()
          : OverridenParamVarDecl->getDefaultArg();

  if (!OverridenDAE || !OverridingDAE)
    return;

  bool OverridenDefaultArgEvaluatable = OverridenDAE->isEvaluatable(ASTCtx);
  bool OverridingDefaultArgEvaluatable = OverridingDAE->isEvaluatable(ASTCtx);

  // Warn if one expression is evaluatable and other one isn't.
  if ((OverridenDefaultArgEvaluatable && !OverridingDefaultArgEvaluatable) ||
      (!OverridenDefaultArgEvaluatable && OverridingDefaultArgEvaluatable)) {
    Locations.insert(OverridingParamVarDecl->getBeginLoc());
    return;
  }

  (OverridenDefaultArgEvaluatable && OverridingDefaultArgEvaluatable)
      ? handleEvaluatableArguments(OverridenDAE, OverridingDAE, Loc)
      : handleUnevaluatableArguments(OverridenDAE, OverridingDAE, Loc);
}

bool ChangedDefaultArgumentsVisitor::VisitCXXMethodDecl(
    const clang::CXXMethodDecl *MD) {
  // Iterate over methods that method MD overrides.
  for (const auto Method : MD->overridden_methods()) {
    auto OverridingParamIter = MD->param_begin();

    for (auto OverridenParamIter = Method->param_begin();
         OverridenParamIter != Method->param_end();
         OverridenParamIter++, OverridingParamIter++)
      checkParameters(*OverridenParamIter, *OverridingParamIter);
  }

  for (const auto &Location : Locations) {
    if (AutocheckDiagnostic::reportWarning(
            DE, Location, AutocheckWarnings::changedDefaultArguments)
            .limitReached())
      return false;
  }
  Locations.clear();
  return true;
}

/* Implementation of ForbiddenOperatorOverloadVisitor */

ForbiddenOperatorOverloadVisitor::ForbiddenOperatorOverloadVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool ForbiddenOperatorOverloadVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::commaAndOrOpsOverloaded);
}

bool ForbiddenOperatorOverloadVisitor::VisitCXXMethodDecl(
    const clang::CXXMethodDecl *CMD) {
  if (CMD->isOverloadedOperator()) {
    switch (CMD->getOverloadedOperator()) {
    case clang::OO_Comma:
    case clang::OO_AmpAmp:
    case clang::OO_PipePipe:
      return !AutocheckDiagnostic::reportWarning(
                  DE, CMD->getLocation(),
                  AutocheckWarnings::commaAndOrOpsOverloaded)
                  .limitReached();
    default:
      return true;
    }
  }
  return true;
}

/* Implementation of UnaryAmpOperatorOverloadVisitor */

UnaryAmpOperatorOverloadVisitor::UnaryAmpOperatorOverloadVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool UnaryAmpOperatorOverloadVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::unaryAmpOpOverloaded);
}

bool UnaryAmpOperatorOverloadVisitor::VisitFunctionDecl(
    const clang::FunctionDecl *FD) {
  if (FD->isOverloadedOperator() &&
      (FD->getOverloadedOperator() == clang::OO_Amp)) {
    return !AutocheckDiagnostic::reportWarning(
                DE, FD->getLocation(), AutocheckWarnings::unaryAmpOpOverloaded)
                .limitReached();
  }
  return true;
}

/* Implementation of AssignmentOpVisitor */

AssignmentOpVisitor::AssignmentOpVisitor(clang::DiagnosticsEngine &DE,
                                         const AutocheckContext &Context)
    : DE(DE), Context(Context),
      CheckRefQual(Context.isEnabled(AutocheckWarnings::assignmentOpRefQual)),
      CheckVirtual(Context.isEnabled(AutocheckWarnings::assignmentOpVirtual)) {}

bool AssignmentOpVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::assignmentOpRefQual) ||
         Context.isEnabled(AutocheckWarnings::assignmentOpVirtual);
}

bool AssignmentOpVisitor::VisitCXXMethodDecl(const clang::CXXMethodDecl *MD) {
  switch (MD->getOverloadedOperator()) {
  case clang::OO_Equal:
  case clang::OO_PlusEqual:
  case clang::OO_MinusEqual:
  case clang::OO_StarEqual:
  case clang::OO_SlashEqual:
  case clang::OO_PercentEqual:
  case clang::OO_AmpEqual:
  case clang::OO_PipeEqual:
  case clang::OO_CaretEqual:
  case clang::OO_LessLessEqual:
  case clang::OO_GreaterGreaterEqual:
    if (CheckRefQual && (MD->getRefQualifier() != clang::RQ_LValue)) {
      CheckRefQual =
          !AutocheckDiagnostic::reportWarning(
               DE, MD->getLocation(), AutocheckWarnings::assignmentOpRefQual)
               .limitReached();
    }
    if (CheckVirtual && MD->isVirtual()) {
      CheckVirtual =
          !AutocheckDiagnostic::reportWarning(
               DE, MD->getLocation(), AutocheckWarnings::assignmentOpVirtual)
               .limitReached();
    }
    break;
  default:
    return true;
  }

  return CheckRefQual || CheckVirtual;
}

/* Implementation of ProperStructureVisitor */

ProperStructureVisitor::ProperStructureVisitor(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool ProperStructureVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::properStructure);
}

bool ProperStructureVisitor::VisitCXXRecordDecl(
    const clang::CXXRecordDecl *CRD) {
  if (!CRD->hasDefinition())
    return true;

  if (CRD->isStruct() && CRD->isCompleteDefinition()) {
    // (4) not inherit from another struct or class.
    // This check is done before (2) so that no implicit method can trigger a
    // warning for (2) which might confuse the user.
    if (!CRD->bases().empty()) {
      return !AutocheckDiagnostic::reportWarning(
                  DE, CRD->getBeginLoc(), AutocheckWarnings::properStructure, 3)
                  .limitReached();
    }

    // (2) not provide any special member functions or methods.
    // This check is done before (1) so we don't have to iterate through every
    // method to check if any of them is not public.
    if (!CRD->methods().empty() &&
        std::any_of(CRD->method_begin(), CRD->method_end(),
                    [](const clang::CXXMethodDecl *CMD) {
                      return !CMD->isImplicit();
                    })) {
      return !AutocheckDiagnostic::reportWarning(
                  DE, CRD->getBeginLoc(), AutocheckWarnings::properStructure, 1)
                  .limitReached();
    }

    // (1) provide only public data members.
    if (CRD->hasPrivateFields() || CRD->hasProtectedFields()) {
      return !AutocheckDiagnostic::reportWarning(
                  DE, CRD->getBeginLoc(), AutocheckWarnings::properStructure, 0)
                  .limitReached();
    }
  }

  // (3) not be a base of another struct or class.
  // This check is done last as it is most complex, in that it needs to iterate
  // through every base class.
  for (const clang::CXXBaseSpecifier &BaseSpec : CRD->bases()) {
    const clang::Type *TypeNode = BaseSpec.getType().getTypePtr();
    const clang::CXXRecordDecl *BaseDecl =
        BaseSpec.getType()->getAsCXXRecordDecl();

    if (BaseDecl && BaseDecl->isStruct()) {
      bool stopVisitor =
          AutocheckDiagnostic::reportWarning(
              DE, CRD->getBeginLoc(), AutocheckWarnings::properStructure, 2)
              .limitReached();
      AutocheckDiagnostic::reportWarning(
          DE, BaseDecl->getBeginLoc(), AutocheckWarnings::noteProperStructure);
      return !stopVisitor;
    }
  }

  return true;
}

/* Implementation of BaseDestructorVisitor */

BaseDestructorVisitor::BaseDestructorVisitor(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool BaseDestructorVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::baseDestructor);
}

bool BaseDestructorVisitor::VisitCXXRecordDecl(
    const clang::CXXRecordDecl *CRD) {
  if (CRD->isImplicit() || !CRD->isCompleteDefinition() ||
      !(CRD->isClass() || CRD->isStruct()))
    return true;

  for (const clang::CXXBaseSpecifier &BaseSpec : CRD->bases()) {
    const clang::Type *TypeNode = BaseSpec.getType().getTypePtr();
    const clang::CXXRecordDecl *BaseDecl = TypeNode->getAsCXXRecordDecl();
    if (BaseDecl) {
      const clang::CXXDestructorDecl *Dtor = BaseDecl->getDestructor();
      if (Dtor) {
        clang::AccessSpecifier AS = Dtor->getAccess();

        // Is destructor public virtual or public override.
        if (AS == clang::AS_public &&
            (Dtor->isVirtual() || Dtor->size_overridden_methods() > 0 ||
             Dtor->hasAttr<clang::OverrideAttr>()))
          continue;

        // Is destructor protected non-virtual.
        if (AS == clang::AS_protected && !Dtor->isVirtual())
          continue;

        // Every other type of destructor is not allowed.
        bool stopVisitor =
            AutocheckDiagnostic::reportWarning(
                DE, CRD->getBeginLoc(), AutocheckWarnings::baseDestructor)
                .limitReached();
        AutocheckDiagnostic::reportWarning(
            DE, Dtor->getBeginLoc(), AutocheckWarnings::noteBaseDestructor,
            Dtor->isImplicit());
        if (stopVisitor)
          return false;
        continue;
      }

      if (BaseDecl->hasSimpleDestructor()) {
        bool stopVisitor =
            AutocheckDiagnostic::reportWarning(
                DE, CRD->getBeginLoc(), AutocheckWarnings::baseDestructor)
                .limitReached();
        AutocheckDiagnostic::reportWarning(
            DE, BaseDecl->getBeginLoc(), AutocheckWarnings::noteBaseDestructor,
            true);
        if (stopVisitor)
          return false;
        continue;
      }
    }
  }

  return true;
}

/* ClassesVisitor */

ClassesVisitor::ClassesVisitor(clang::DiagnosticsEngine &DE,
                               clang::ASTContext &ASTCtx)
    : DE(DE) {
  const AutocheckContext &Context = AutocheckContext::Get();
  if (DerivedFromVirtualVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<DerivedFromVirtualVisitor>(DE));
  if (VirtualFuncSpecifierVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<VirtualFuncSpecifierVisitor>(DE));
  if (OverrideFuncSpecifierVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<OverrideFuncSpecifierVisitor>(DE));
  if (VirtualMethodFinalClassVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<VirtualMethodFinalClassVisitor>(DE));
  if (PureFuncOverrideVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<PureFuncOverrideVisitor>(DE));
  if (MemberDataPrivateVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<MemberDataPrivateVisitor>(DE));
  if (NonPODAsClassVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<NonPODAsClassVisitor>(DE));
  if (ExplicitCtorsVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<ExplicitCtorsVisitor>(DE));
  if (FinalClassDestructorVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<FinalClassDestructorVisitor>(DE));
  if (RelOpBoolValueVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<RelOpBoolValueVisitor>(DE));
  if (ImplicitConversionOpVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<ImplicitConversionOpVisitor>(DE));
  if (ConverionOpUsedVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<ConverionOpUsedVisitor>(DE));
  if (SubscriptOperatorOverloadVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<SubscriptOperatorOverloadVisitor>(DE));
  if (UnusedPrivateMethodVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<UnusedPrivateMethodVisitor>(DE));
  if (NonConstReturnedFromConst::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<NonConstReturnedFromConst>(DE));
  if (CopyAndMoveAssignmentNoSwapVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<CopyAndMoveAssignmentNoSwapVisitor>(DE));
  if (UnionsUsedVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<UnionsUsedVisitor>(DE, ASTCtx));
  if (ChangedDefaultArgumentsVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<ChangedDefaultArgumentsVisitor>(DE, ASTCtx));
  if (ForbiddenOperatorOverloadVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<ForbiddenOperatorOverloadVisitor>(DE));
  if (UnaryAmpOperatorOverloadVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<UnaryAmpOperatorOverloadVisitor>(DE));
  if (AssignmentOpVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<AssignmentOpVisitor>(DE, Context));
  if (ProperStructureVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<ProperStructureVisitor>(DE));
  if (BaseDestructorVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<BaseDestructorVisitor>(DE));
}

void ClassesVisitor::run(clang::TranslationUnitDecl *TUD) {
  if (!AllVisitors.empty()) {
    this->TraverseDecl(TUD);
    for (auto &Visitor : AllVisitors)
      Visitor->PostWork();
  }
}

bool ClassesVisitor::TraverseDecl(clang::Decl *D) {
  if (!D) // Can be null if code had errors.
    return true;

  clang::SourceLocation Loc = D->getBeginLoc();

  if (Loc.isInvalid() || appropriateHeaderLocation(DE, Loc)) {
    AllVisitors.remove_if([D](std::unique_ptr<ClassesVisitorInterface> &V) {
      return !V->PreTraverseDecl(D);
    });

    clang::RecursiveASTVisitor<ClassesVisitor>::TraverseDecl(D);

    AllVisitors.remove_if([D](std::unique_ptr<ClassesVisitorInterface> &V) {
      return !V->PostTraverseDecl(D);
    });
  }
  return true;
}

bool ClassesVisitor::VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) {
  AllVisitors.remove_if([CRD](std::unique_ptr<ClassesVisitorInterface> &V) {
    return !V->VisitCXXRecordDecl(CRD);
  });
  return true;
}

bool ClassesVisitor::VisitCXXMethodDecl(const clang::CXXMethodDecl *CMD) {
  AllVisitors.remove_if([CMD](std::unique_ptr<ClassesVisitorInterface> &V) {
    return !V->VisitCXXMethodDecl(CMD);
  });
  return true;
}

bool ClassesVisitor::VisitCXXConversionDecl(
    const clang::CXXConversionDecl *CCD) {
  AllVisitors.remove_if([CCD](std::unique_ptr<ClassesVisitorInterface> &V) {
    return !V->VisitCXXConversionDecl(CCD);
  });
  return true;
}

bool ClassesVisitor::VisitCXXMemberCallExpr(
    const clang::CXXMemberCallExpr *CMCE) {
  AllVisitors.remove_if([CMCE](std::unique_ptr<ClassesVisitorInterface> &V) {
    return !V->VisitCXXMemberCallExpr(CMCE);
  });
  return true;
}

bool ClassesVisitor::VisitFunctionDecl(const clang::FunctionDecl *FD) {
  AllVisitors.remove_if([FD](std::unique_ptr<ClassesVisitorInterface> &V) {
    return !V->VisitFunctionDecl(FD);
  });
  return true;
}

} // namespace autocheck
