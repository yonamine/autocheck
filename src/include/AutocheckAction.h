//===--- AutocheckAction.h - Main autocheck tool action -------------------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file declares the main autocheck frontend action class which performs
// all AUTOSAR rule checking.
//
//===----------------------------------------------------------------------===//

#ifndef AUTOCHECK_ACTION_H
#define AUTOCHECK_ACTION_H

#include "Diagnostics/AutocheckDiagnostic.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"

namespace autocheck {

class AutocheckContext;
class AutocheckPPCallbacks;

class AutocheckAction : public clang::FrontendAction {
public:
  AutocheckAction(const AutocheckContext &Context);

  bool usesPreprocessorOnly() const override;

protected:
  bool BeginInvocation(clang::CompilerInstance &CI) override;

  void ExecuteAction() override;

  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &CI,
                    llvm::StringRef InFile) override;

private:
  const AutocheckContext &Context;
  std::unique_ptr<AutocheckDiagnostic> AD;

  // Preprocessor callbacks. Owned by the preprocessor.
  AutocheckPPCallbacks *PPCallbacks;
};

} // namespace autocheck

#endif
