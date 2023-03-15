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
#include "clang/Basic/DiagnosticParse.h"
#include "clang/Basic/DiagnosticSema.h"

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

void AutocheckDiagnosticConsumer::EmitDiag(AutocheckWarnings Warning,
                                           const clang::SourceLocation &Loc) {
  Diags.Clear();
  AutocheckDiagnostic::reportWarning(Diags, Loc, Warning);
}

void AutocheckDiagnosticConsumer::HandleDiagnostic(
    clang::DiagnosticsEngine::Level DiagLevel, const clang::Diagnostic &Info) {
  // Convert built-in warnings to autocheck warnings.
  switch (Info.getID()) {
  case clang::diag::ext_embedded_directive:
    EmitDiag(AutocheckWarnings::embeddedDirective, Info.getLocation());
    return;
  case clang::diag::trigraph_ignored:
  case clang::diag::trigraph_converted:
    Diags.Clear();
    EmitDiag(AutocheckWarnings::trigraphsUsed, Info.getLocation());
    return;
  case clang::diag::warn_nested_block_comment:
    EmitDiag(AutocheckWarnings::commentStartInComment, Info.getLocation());
    return;
  case clang::diag::warn_pp_macro_is_reserved_id:
  case clang::diag::ext_pp_redef_builtin_macro:
  case clang::diag::ext_pp_undef_builtin_macro:
    EmitDiag(AutocheckWarnings::reservedIdentifiers, Info.getLocation());
    return;
  case clang::diag::warn_unused_function:
    EmitDiag(AutocheckWarnings::unusedFunctionOrMethod, Info.getLocation());
    return;
  case clang::diag::warn_unused_local_typedef:
    EmitDiag(AutocheckWarnings::unusedTypedef, Info.getLocation());
    return;
  case clang::diag::warn_exception_spec_deprecated:
  case clang::diag::ext_dynamic_exception_spec:
    EmitDiag(AutocheckWarnings::deprecatedDynamicExceptionSpec,
             Info.getLocation());
    return;
  case clang::diag::note_exception_spec_deprecated:
    return;
  }

  Client->HandleDiagnostic(DiagLevel, Info);
}

} // namespace autocheck
