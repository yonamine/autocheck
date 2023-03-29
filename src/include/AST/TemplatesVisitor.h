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
// - [A8-4-7] "in" parameters for "cheap to copy" types shall be passed by
//            value.
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
};

/// [A8-4-7] "in" parameters for "cheap to copy" types shall be passed by value.
class InParametersPassedByValue : public TemplateVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &AC;

  bool checkParameter(const clang::ParmVarDecl *PVD,
                      const clang::SourceLocation &InstantiationLoc);
  // We consider that class is not cheap to copy if it has a field with a
  // pointer type.
  bool classHasPointerField(const clang::Type *T);

public:
  explicit InParametersPassedByValue(clang::DiagnosticsEngine &DE,
                                     clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitFunctionDecl(const clang::FunctionDecl *FD) override;
};

/// Main visitor for checks that involve template instantiations. Makes an
/// instance of every class that implement a DeclarationsVisitorInterface if
/// appropriate flag is found. Runs all Declaration Visitors with one AST
/// traversal.
class TemplatesVisitor : public clang::RecursiveASTVisitor<TemplatesVisitor> {
  clang::DiagnosticsEngine &DE;

  std::forward_list<std::unique_ptr<TemplateVisitorInterface>> AllVisitors;

public:
  explicit TemplatesVisitor(clang::DiagnosticsEngine &DE,
                            clang::ASTContext &AC);
  void run(clang::TranslationUnitDecl *TUD);

  bool shouldVisitTemplateInstantiations() const { return true; }
  bool VisitFunctionDecl(const clang::FunctionDecl *FD);
};

} // namespace autocheck

#endif
