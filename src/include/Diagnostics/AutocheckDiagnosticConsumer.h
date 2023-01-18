//===- AutocheckDiagnosticConsumer.h - Diagnostic handler -----------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares a DiagnosticConsumer used for converting clang warnings to
// autocheck warnings. It takes the place of an existing consumer, intercepts
// warnings and converts them if necessary before passing the new diagnostic to
// the original consumer.
//
//===----------------------------------------------------------------------===//

#ifndef DIAGNOSTICS_AUTOCHECK_DIAGNOSTIC_CONSUMER_H
#define DIAGNOSTICS_AUTOCHECK_DIAGNOSTIC_CONSUMER_H

#include "clang/Basic/Diagnostic.h"
#include <memory>

namespace autocheck {

class AutocheckDiagnosticConsumer : public clang::DiagnosticConsumer {
public:
  AutocheckDiagnosticConsumer(clang::DiagnosticsEngine &Diags);

  void BeginSourceFile(const clang::LangOptions &LangOpts,
                       const clang::Preprocessor *PP = nullptr) override;

  void EndSourceFile() override;

  void HandleDiagnostic(clang::DiagnosticsEngine::Level DiagLevel,
                        const clang::Diagnostic &Info) override;

private:
  clang::DiagnosticsEngine &Diags;
  clang::DiagnosticConsumer *Client;
  std::unique_ptr<clang::DiagnosticConsumer> ClientOwner;
};

} // namespace autocheck

#endif
