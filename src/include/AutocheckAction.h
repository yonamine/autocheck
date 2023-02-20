//===--- AutocheckAction.h - Main autocheck tool action -------------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the main autocheck frontend action class which performs
// all AUTOSAR rule checking.
//
//===----------------------------------------------------------------------===//

#ifndef AUTOCHECK_ACTION_H
#define AUTOCHECK_ACTION_H

#include "AutocheckContext.h"
#include "Lex/AutocheckPPCallbacks.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"

namespace autocheck {

class AutocheckAction : public clang::FrontendAction {
public:
  AutocheckAction();

  bool usesPreprocessorOnly() const override;

protected:
  bool BeginInvocation(clang::CompilerInstance &CI) override;

  bool BeginSourceFileAction(clang::CompilerInstance &CI) override;

  void ExecuteAction() override;

  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &CI,
                    llvm::StringRef InFile) override;

private:
  AutocheckContext &Context;
  const AutocheckPPCallbacks *PPCallbacks;
};

// This class performs all Autosar rule checks on the generated AST.
class AutocheckASTConsumer : public clang::ASTConsumer {
public:
  AutocheckASTConsumer(const AutocheckPPCallbacks &PPCallbacks);

  void HandleTranslationUnit(clang::ASTContext &Context) override;

private:
  const AutocheckPPCallbacks &PPCallbacks;
};

} // namespace autocheck

#endif
