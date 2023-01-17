//===--- AutocheckDiagnosticConsumer.cpp - Diagnostic handler -------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the AutocheckDiagnosticConsumer class.
//
//===----------------------------------------------------------------------===//

#include "Diagnostics/AutocheckDiagnosticConsumer.h"

#include "Diagnostics/AutocheckDiagnostic.h"
#include "clang/Basic/DiagnosticLex.h"

namespace autocheck {

AutocheckDiagnosticConsumer::AutocheckDiagnosticConsumer(
    clang::DiagnosticsEngine &Diags)
    : Diags(Diags), Client(Diags.getClient()), ClientOwner(Diags.takeClient()) {
}

void AutocheckDiagnosticConsumer::BeginSourceFile(
    const clang::LangOptions &LangOpts, const clang::Preprocessor *PP) {
  Client->BeginSourceFile(LangOpts, PP);
}

void AutocheckDiagnosticConsumer::EndSourceFile() {
  Client->EndSourceFile();
  // Take number of errors and warnings from parent consumer.
  // These are used to determine the exit code of the process.
  NumErrors = Client->getNumErrors();
  NumWarnings = Client->getNumWarnings();
}

void AutocheckDiagnosticConsumer::HandleDiagnostic(
    clang::DiagnosticsEngine::Level DiagLevel, const clang::Diagnostic &Info) {
  // Errors and fatal diagnostics are handled by parent consumer.
  if (DiagLevel > clang::DiagnosticsEngine::Level::Warning) {
    Client->HandleDiagnostic(DiagLevel, Info);
    return;
  }

  // Convert built-in warnings to autocheck warnings.
  switch (Info.getID()) {
    // TODO: Convert warnings.
  }

  Client->HandleDiagnostic(DiagLevel, Info);
}

} // namespace autocheck
