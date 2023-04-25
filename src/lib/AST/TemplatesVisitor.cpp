//===--- TemplatesVisitor.cpp - Template instantiation checks visitor -----===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the TemplatesVisitor class and all classes derived from
// TemplateVisitorInterface.
//
//===----------------------------------------------------------------------===//

#include "AST/TemplatesVisitor.h"

#include "AST/SideEffectChecker.h"
#include "Diagnostics/AutocheckDiagnostic.h"

namespace autocheck {

/* Implementation of TemplateVisitorInterface */

TemplateVisitorInterface::~TemplateVisitorInterface() {}

using TVI = TemplateVisitorInterface;
bool TVI::VisitFunctionDecl(const clang::FunctionDecl *) { return true; }
bool TVI::VisitBinaryOperator(const clang::BinaryOperator *) { return true; }

/* Implementation of InParametersPassedByValue */

static bool isIncompleteOrDependentType(const clang::Type *T) {
  return (T->isIncompleteType() || T->isDependentType());
}

InParametersPassedByValue::InParametersPassedByValue(
    clang::DiagnosticsEngine &DE, clang::ASTContext &AC)
    : DE(DE), AC(AC) {}

bool InParametersPassedByValue::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::inParamPassedByValue);
}

bool InParametersPassedByValue::VisitFunctionDecl(
    const clang::FunctionDecl *FD) {
  clang::SourceLocation InstantiationLoc;
  // Always allow copy constructors.
  if (const clang::CXXConstructorDecl *CD =
          llvm::dyn_cast_if_present<clang::CXXConstructorDecl>(FD))
    if (CD->isCopyConstructor())
      return true;
  const auto &Parameters = FD->parameters();
  unsigned NumberOfParameters = FD->getNumParams();
  for (unsigned i = 0; i < NumberOfParameters; ++i) {
    if (FD->isTemplateInstantiation()) {
      if (FD->getTemplateSpecializationInfo())
        InstantiationLoc =
            FD->getTemplateSpecializationInfo()->getPointOfInstantiation();
    }
    if (!checkParameter(Parameters[i], InstantiationLoc))
      return false;
  }

  return true;
}

bool InParametersPassedByValue::classHasPointerField(const clang::Type *T) {
  if (auto CXXRD = T->getAsCXXRecordDecl()) {
    for (auto field : CXXRD->fields()) {
      auto T = field->getType();
      if (T->isPointerType())
        return true;
    }
  }
  return false;
}

bool InParametersPassedByValue::checkParameter(
    const clang::ParmVarDecl *PVD,
    const clang::SourceLocation &InstantiationLoc) {
  const clang::SourceLocation &SL = PVD->getBeginLoc();
  // Get and check QualType.
  const clang::QualType &QT = PVD->getType();
  if (QT.isNull()) {
    return true;
  }
  // Get and check Type.
  // The "getTypeSize" method cannot work with incomplete or dependent types.
  const clang::Type *T = QT.getTypePtrOrNull();
  if (!T || isIncompleteOrDependentType(T)) {
    return true;
  }

  // If PVD belongs to template instantiation but its type doesn't depend on
  // template, leave. This prevents multiple warnings for types that are not
  // templated but are part of declaration of template function for example:
  // template <typename T>
  // void f(int32_t a, const T& b)
  if (InstantiationLoc.isValid()) {
    if (!QT.getNonReferenceType()->getAs<clang::SubstTemplateTypeParmType>())
      return true;
  }
  // Get pointer size in bits.
  const auto PtrWidth = sizeof(void *) * 8;

  // Process reference type.
  if (T->isLValueReferenceType()) {
    // Get and check underlying QualType.
    const clang::QualType &NRQT = QT.getNonReferenceType();
    if (NRQT.isNull()) {
      return true;
    }
    // Process const reference type.
    if (NRQT.isConstQualified()) {
      // Get and check underlying Type.
      const clang::Type *NRT = NRQT.getTypePtrOrNull();

      if (!NRT || isIncompleteOrDependentType(NRT) || classHasPointerField(NRT))
        return true;

      if (AC.getTypeSize(NRT) <= PtrWidth) {
        bool stopVisitor = AutocheckDiagnostic::reportWarning(
                               DE, SL, AutocheckWarnings::inParamPassedByValue,
                               0, NRQT.getUnqualifiedType())
                               .limitReached();
        if (InstantiationLoc.isValid()) {
          AutocheckDiagnostic::reportWarning(
              DE, InstantiationLoc, AutocheckWarnings::noteInParamPassedByValue,
              NRQT.getUnqualifiedType());
        }
        return !stopVisitor;
      }
    }
    // Process non-reference type.
  } else {
    if (AC.getTypeSize(T) > PtrWidth || classHasPointerField(T)) {
      bool stopVisitor = AutocheckDiagnostic::reportWarning(
                             DE, SL, AutocheckWarnings::inParamPassedByValue, 1,
                             QT.getUnqualifiedType())
                             .limitReached();
      if (InstantiationLoc.isValid())
        AutocheckDiagnostic::reportWarning(
            DE, InstantiationLoc, AutocheckWarnings::noteInParamPassedByValue,
            QT.getUnqualifiedType());
      return !stopVisitor;
    }
  }

  return true;
}

/* Implementation of RHSOperandSideEffectVisitor */

RHSOperandSideEffectVisitor::RHSOperandSideEffectVisitor(
    clang::DiagnosticsEngine &DE, clang::ASTContext &AC)
    : DE(DE), AC(AC) {}

bool RHSOperandSideEffectVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::rhsOperandAndOrSideEffect);
}

bool RHSOperandSideEffectVisitor::VisitBinaryOperator(
    const clang::BinaryOperator *BO) {
  if (BO->getRHS()->isInstantiationDependent())
    return true;

  if (BO->getOpcode() == clang::BO_LAnd || BO->getOpcode() == clang::BO_LOr) {
    if (hasAutosarSideEffects(BO->getRHS(), AC)) {
      return !AutocheckDiagnostic::reportWarning(
                  DE, BO->getRHS()->getBeginLoc(),
                  AutocheckWarnings::rhsOperandAndOrSideEffect)
                  .limitReached();
    }
  }

  return true;
}

/* Implementation of TemplatesVisitor */

TemplatesVisitor::TemplatesVisitor(clang::DiagnosticsEngine &DE,
                                         clang::ASTContext &AC)
    : DE(DE) {
  const AutocheckContext &Context = AutocheckContext::Get();
  if (InParametersPassedByValue::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<InParametersPassedByValue>(DE, AC));
  if (RHSOperandSideEffectVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<RHSOperandSideEffectVisitor>(DE, AC));
    }

void TemplatesVisitor::run(clang::TranslationUnitDecl *TUD) {
  if (!AllVisitors.empty()) {
    this->TraverseDecl(TUD);
  }
}

bool TemplatesVisitor::VisitFunctionDecl(const clang::FunctionDecl *FD) {
  AllVisitors.remove_if([FD](std::unique_ptr<TemplateVisitorInterface> &V) {
    return !V->VisitFunctionDecl(FD);
  });
  return true;
}

bool TemplatesVisitor::VisitBinaryOperator(const clang::BinaryOperator *BO) {
  AllVisitors.remove_if([BO](std::unique_ptr<TemplateVisitorInterface> &V) {
    return !V->VisitBinaryOperator(BO);
  });
  return true;
}

} // namespace autocheck
