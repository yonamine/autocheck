//===--- AutocheckAction.cpp - Main autocheck tool action -----------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the AutocheckAction class.
//
//===----------------------------------------------------------------------===//

#include "AutocheckAction.h"

#include "Diagnostics/AutocheckDiagnosticConsumer.h"
#include "llvm/Support/raw_ostream.h"

namespace autocheck {

AutocheckAction::AutocheckAction(AutocheckContext &Context)
    : Context(Context){};

bool AutocheckAction::BeginInvocation(clang::CompilerInstance &CI) {
  AutocheckDiagnosticConsumer *DiagConsumer =
      new AutocheckDiagnosticConsumer(CI.getDiagnostics());
  CI.getDiagnostics().setClient(DiagConsumer, /*ShouldOwnClient=*/true);
  return true;
}

void AutocheckAction::ExecuteAction() {
  // TODO: Perform checks.
  llvm::outs() << "Processing " << getCurrentFile() << "\n";
}

} // namespace autocheck
