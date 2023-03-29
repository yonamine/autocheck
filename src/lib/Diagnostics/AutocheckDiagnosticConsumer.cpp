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
#include "clang/AST/Type.h"
#include "clang/Basic/DiagnosticLex.h"
#include "clang/Basic/DiagnosticParse.h"
#include "clang/Basic/DiagnosticSema.h"
#include "clang/Sema/Ownership.h"

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
  case clang::diag::warn_old_style_cast:
    EmitDiag(AutocheckWarnings::cStyleCastUsed, Info.getLocation());
    return;
  case clang::diag::warn_cast_qual: {
    // Read diagnostic parameters.
    clang::QualType FromType =
        clang::QualType::getFromOpaquePtr((void *)Info.getRawArg(0));
    clang::QualType ToType =
        clang::QualType::getFromOpaquePtr((void *)Info.getRawArg(1));
    int64_t Qualifiers = Info.getArgSInt(2);

    EmitDiag(AutocheckWarnings::castRemovesQual, Info.getLocation());

    Diags.Clear();
    AutocheckDiagnostic::reportWarning(Diags, Info.getLocation(),
                                       AutocheckWarnings::noteCastRemovesQual,
                                       0, FromType, ToType, Qualifiers);

    return;
  }
  case clang::diag::warn_cast_qual2: {
    // Read diagnostic parameters.
    clang::ActionResult<clang::Expr *> E =
        clang::ActionResult<clang::Expr *>::getFromOpaquePointer(
            (void *)Info.getRawArg(0));
    clang::QualType ToType =
        clang::QualType::getFromOpaquePtr((void *)Info.getRawArg(1));

    Diags.Clear();
    AutocheckDiagnostic::reportWarning(Diags, Info.getLocation(),
                                       AutocheckWarnings::noteCastRemovesQual,
                                       1, E.get()->getType(), ToType);

    return;
  }
  case clang::diag::warn_unused_parameter: {
    // Read diagnostic parameters.
    clang::DeclarationName Name =
        clang::DeclarationName::getFromOpaqueInteger(Info.getRawArg(0));

    EmitDiag(AutocheckWarnings::unusedParameter, Info.getLocation());

    Diags.Clear();
    AutocheckDiagnostic::reportWarning(Diags, Info.getLocation(),
                                       AutocheckWarnings::noteUnusedParameter,
                                       Name);

    return;
  }
  case clang::diag::warn_exception_caught_by_earlier_handler:
    EmitDiag(AutocheckWarnings::exceptionHandlerInversion, Info.getLocation());
    return;
  case clang::diag::note_previous_exception_handler:
    EmitDiag(AutocheckWarnings::noteExceptionHandlerInversion,
             Info.getLocation());
    return;
  case clang::diag::warn_cdtor_function_try_handler_mem_expr:
  case clang::diag::warn_throw_in_noexcept_func:
    // Mute excess diagnostics enabled by -Wexception
    return;
  case clang::diag::warn_ret_stack_addr_ref:
  case clang::diag::warn_ret_addr_label:
  case clang::diag::warn_ret_local_temp_addr_ref:
    EmitDiag(AutocheckWarnings::returnStackAddress, Info.getLocation());
    return;
  case clang::diag::warn_falloff_nonvoid_coroutine:
  case clang::diag::warn_falloff_nonvoid_function:
  case clang::diag::warn_falloff_nonvoid_lambda:
  case clang::diag::warn_maybe_falloff_nonvoid_coroutine:
  case clang::diag::warn_maybe_falloff_nonvoid_function:
  case clang::diag::warn_maybe_falloff_nonvoid_lambda:
    EmitDiag(AutocheckWarnings::returnNonVoidFunction, Info.getLocation());
    return;
  case clang::diag::warn_falloff_noreturn_function:
  case clang::diag::err_falloff_nonvoid_block:
  case clang::diag::err_maybe_falloff_nonvoid_block:
    // Mute excess diagnostics enabled by -Wreturn-type
    return;
  case clang::diag::warn_unannotated_fallthrough:
    EmitDiag(AutocheckWarnings::breakSwitchCase, Info.getLocation());
    return;
  case clang::diag::warn_unannotated_fallthrough_per_function:
  case clang::diag::note_insert_break_fixit:
    // Mute excess diagnostics enabled by -Wimplicit-fallthrough
    return;
  case clang::diag::warn_field_is_uninit:
  case clang::diag::warn_base_class_is_uninit:
  case clang::diag::warn_reference_field_is_uninit:
  case clang::diag::warn_static_self_reference_in_init:
  case clang::diag::warn_uninit_self_reference_in_init:
  case clang::diag::warn_uninit_self_reference_in_reference_init:
  case clang::diag::warn_uninit_var:
  case clang::diag::warn_uninit_byref_blockvar_captured_by_block:
  case clang::diag::warn_sometimes_uninit_var:
  case clang::diag::warn_uninit_const_reference:
    // TODO: Re-emit as custom diagnostic.
    break;
  }

  Client->HandleDiagnostic(DiagLevel, Info);
}

} // namespace autocheck
