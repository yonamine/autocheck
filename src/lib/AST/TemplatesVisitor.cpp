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
#include "clang/AST/QualTypeNames.h"
#include "clang/Analysis/Analyses/ReachableCode.h"
#include "clang/Analysis/AnalysisDeclContext.h"
#include "clang/Sema/Sema.h"

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
  const auto CheapTypeLimit = 2 * sizeof(void *) * 8;

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

      if (!NRT || isIncompleteOrDependentType(NRT))
        return true;

      if (AC.getTypeSize(NRT) <= CheapTypeLimit) {
        bool stopVisitor =
            AutocheckDiagnostic::reportWarning(
                DE, SL, AutocheckWarnings::inParamPassedByValue, 0,
                clang::TypeName::getFullyQualifiedType(
                    NRQT.getLocalUnqualifiedType(), AC))
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
    if (AC.getTypeSize(T) > CheapTypeLimit) {
      bool stopVisitor = AutocheckDiagnostic::reportWarning(
                             DE, SL, AutocheckWarnings::inParamPassedByValue, 1,
                             clang::TypeName::getFullyQualifiedType(
                                 QT.getLocalUnqualifiedType(), AC))
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

/* Implementation of ThrowEscapesVisitor */

ThrowEscapesVisitor::ThrowEscapesVisitor(clang::DiagnosticsEngine &DE,
                                         clang::Sema &SemaRef)
    : DE(DE), SemaRef(SemaRef) {}

bool ThrowEscapesVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::handlerOfCompatibleType);
}

/// Determine whether an exception thrown by E, unwinding from ThrowBlock,
/// can reach ExitBlock.
///
/// This implementation was taken from clang/lib/Sema/AnalysisBasedWarnings.cpp
static bool throwEscapes(clang::Sema &S, const clang::CXXThrowExpr *E,
                         clang::CFGBlock &ThrowBlock, clang::CFG *Body) {
  llvm::SmallVector<clang::CFGBlock *, 16> Stack;
  llvm::BitVector Queued(Body->getNumBlockIDs());

  Stack.push_back(&ThrowBlock);
  Queued[ThrowBlock.getBlockID()] = true;

  while (!Stack.empty()) {
    clang::CFGBlock &UnwindBlock = *Stack.back();
    Stack.pop_back();

    for (auto &Succ : UnwindBlock.succs()) {
      if (!Succ.isReachable() || Queued[Succ->getBlockID()])
        continue;

      if (Succ->getBlockID() == Body->getExit().getBlockID())
        return true;

      if (auto *Catch =
              llvm::dyn_cast_or_null<clang::CXXCatchStmt>(Succ->getLabel())) {
        clang::QualType Caught = Catch->getCaughtType();
        if (Caught.isNull() ||  // catch (...) catches everything
            !E->getSubExpr() || // throw; is considered cuaght by any handler
            S.handlerCanCatch(Caught, E->getSubExpr()->getType()))
          // Exception doesn't escape via this path.
          break;
      } else {
        Stack.push_back(Succ);
        Queued[Succ->getBlockID()] = true;
      }
    }
  }

  return false;
}

/// Visit all reachable throw statements for the given CFG.
///
/// This implementation was taken from clang/lib/Sema/AnalysisBasedWarnings.cpp
static void visitReachableThrows(
    clang::CFG *BodyCFG,
    llvm::function_ref<void(const clang::CXXThrowExpr *, clang::CFGBlock &)>
        Visit) {
  llvm::BitVector Reachable(BodyCFG->getNumBlockIDs());
  clang::reachable_code::ScanReachableFromBlock(&BodyCFG->getEntry(),
                                                Reachable);
  for (clang::CFGBlock *B : *BodyCFG) {
    if (!Reachable[B->getBlockID()])
      continue;
    for (clang::CFGElement &E : *B) {
      llvm::Optional<clang::CFGStmt> S = E.getAs<clang::CFGStmt>();
      if (!S)
        continue;
      if (auto *Throw = llvm::dyn_cast<clang::CXXThrowExpr>(S->getStmt()))
        Visit(Throw, *B);
    }
  }
}

bool ThrowEscapesVisitor::VisitFunctionDecl(const clang::FunctionDecl *FD) {
  // Skip template declarations with unresolved types. They will be checked when
  // instantiated.
  if (FD->getDeclContext()->isDependentContext())
    return true;

  // Construct the analysis context with the specified CFG build options.
  clang::AnalysisDeclContext AC(/* AnalysisDeclContextManager */ nullptr, FD);

  // Don't generate EH edges for CallExprs as we'd like to avoid the n^2
  // explosion for destructors that can result and the compile time hit.
  AC.getCFGBuildOptions().PruneTriviallyFalseEdges = true;
  AC.getCFGBuildOptions().AddEHEdges = false;
  AC.getCFGBuildOptions().setAllAlwaysAdd();

  clang::CFG *BodyCFG = AC.getCFG();
  if (!BodyCFG)
    return true;
  if (BodyCFG->getExit().pred_empty())
    return true;

  bool stopVisitor = false;
  visitReachableThrows(
      BodyCFG, [&](const clang::CXXThrowExpr *Throw, clang::CFGBlock &Block) {
        if (!stopVisitor && throwEscapes(SemaRef, Throw, Block, BodyCFG)) {
          stopVisitor = AutocheckDiagnostic::reportWarning(
                            DE, Throw->getExprLoc(),
                            AutocheckWarnings::handlerOfCompatibleType)
                            .limitReached();
        }
      });

  return !stopVisitor;
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
                                   clang::ASTContext &AC, clang::Sema &SemaRef)
    : DE(DE) {
  const AutocheckContext &Context = AutocheckContext::Get();
  if (InParametersPassedByValue::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<InParametersPassedByValue>(DE, AC));
  if (ThrowEscapesVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<ThrowEscapesVisitor>(DE, SemaRef));
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
