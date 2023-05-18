//===--- TemplatesVisitor.h - Template instantiation checks visitor -------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file performs checks for rules that require visiting template
// instantiations by traversing the clang AST.
//
// This implements the following checks:
// - [M5-14-1] The right hand operand of a logical &&, || operators shall not
///            contain side effects.
// - [A8-4-7]  "in" parameters for "cheap to copy" types shall be passed by
//             value.
// - [M15-3-4] Each exception explicitly thrown in the code shall have a handler
//             of a compatible type in all call paths that could lead to that
//             point.
//
//===----------------------------------------------------------------------===//

#ifndef AST_TEMPLATE_VISITOR_H
#define AST_TEMPLATE_VISITOR_H

#include "AutocheckContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/SourceManager.h"
#include <forward_list>

namespace autocheck {

/// Common interface for all visitors that check template instantiations.
class TemplateVisitorInterface {
public:
  virtual ~TemplateVisitorInterface();

  virtual bool VisitFunctionDecl(const clang::FunctionDecl *FD);
  virtual bool VisitBinaryOperator(const clang::BinaryOperator *BO);
};

/// [A8-4-7] "in" parameters for "cheap to copy" types shall be passed by value.
class InParametersPassedByValue : public TemplateVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &AC;

  bool checkParameter(const clang::ParmVarDecl *PVD,
                      const clang::SourceLocation &InstantiationLoc);

public:
  explicit InParametersPassedByValue(clang::DiagnosticsEngine &DE,
                                     clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitFunctionDecl(const clang::FunctionDecl *FD) override;
};

/// [M15-3-4] Each exception explicitly thrown in the code shall have a handler
/// of a compatible type in all call paths that could lead to that point.
class ThrowEscapesVisitor : public TemplateVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::Sema &SemaRef;

public:
  explicit ThrowEscapesVisitor(clang::DiagnosticsEngine &DE,
                               clang::Sema &SemaRef);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitFunctionDecl(const clang::FunctionDecl *FD) override;
};

/// [M5-14-1] The right hand operand of a logical &&, || operators shall not
/// contain side effects.
class RHSOperandSideEffectVisitor : public TemplateVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &AC;

public:
  explicit RHSOperandSideEffectVisitor(clang::DiagnosticsEngine &DE,
                                       clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitBinaryOperator(const clang::BinaryOperator *BO) override;
};

/// Main visitor for checks that involve template instantiations. Makes an
/// instance of every class that implement a DeclarationsVisitorInterface if
/// appropriate flag is found. Runs all Declaration Visitors with one AST
/// traversal.
class TemplatesVisitor : public clang::RecursiveASTVisitor<TemplatesVisitor> {
  clang::DiagnosticsEngine &DE;

  std::forward_list<std::unique_ptr<TemplateVisitorInterface>> AllVisitors;

public:
  explicit TemplatesVisitor(clang::DiagnosticsEngine &DE, clang::ASTContext &AC,
                            clang::Sema &SemaRef);
  void run(clang::TranslationUnitDecl *TUD);

  bool shouldVisitTemplateInstantiations() const { return true; }
  bool VisitFunctionDecl(const clang::FunctionDecl *FD);
  bool VisitBinaryOperator(const clang::BinaryOperator *BO);
};

} // namespace autocheck

#endif
