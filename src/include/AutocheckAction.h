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
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"

namespace autocheck {

class AutocheckAction : public clang::PreprocessorFrontendAction {
public:
  AutocheckAction(AutocheckContext &Context);

protected:
  bool BeginInvocation(clang::CompilerInstance &CI) override;

  void ExecuteAction() override;

private:
  AutocheckContext &Context;
};

} // namespace autocheck

#endif
