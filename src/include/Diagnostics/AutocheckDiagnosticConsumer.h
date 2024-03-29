//===- AutocheckDiagnosticConsumer.h - Diagnostic handler -----------------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
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

#include "Diagnostics/AutocheckDiagnostic.h"
#include "Diagnostics/AutocheckWarnings.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/SourceLocation.h"
#include <memory>

namespace autocheck {

class AutocheckDiagnosticConsumer : public clang::DiagnosticConsumer {
public:
  AutocheckDiagnosticConsumer(AutocheckDiagnostic &AD);

  void BeginSourceFile(const clang::LangOptions &LangOpts,
                       const clang::Preprocessor *PP = nullptr) override;

  void EndSourceFile() override;

  void HandleDiagnostic(clang::DiagnosticsEngine::Level DiagLevel,
                        const clang::Diagnostic &Info) override;

private:
  template <typename... ArgsType>
  void EmitDiag(AutocheckWarnings Warning, const clang::SourceLocation &Loc,
                ArgsType &&...Args) {
    AD.GetDiagnostics().Clear();
    AD.reportWarning(Loc, Warning, std::forward<ArgsType>(Args)...);
  }

private:
  AutocheckDiagnostic &AD;
  clang::DiagnosticConsumer *Client;
  std::unique_ptr<clang::DiagnosticConsumer> ClientOwner;
};

} // namespace autocheck

#endif
