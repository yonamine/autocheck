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

#include "AutocheckContext.h"
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
  case clang::diag::warn_unused_function: {
    // Read diagnostic parameters.
    clang::NamedDecl *Name =
        reinterpret_cast<clang::NamedDecl *>(Info.getRawArg(0));

    // Emit only one of unusedFunctionOrMethod or unusedFunction.
    if (AutocheckContext::Get().isEnabled(
            AutocheckWarnings::unusedFunctionOrMethod)) {
      Diags.Clear();
      AutocheckDiagnostic::reportWarning(
          Diags, Info.getLocation(), AutocheckWarnings::unusedFunctionOrMethod,
          0, Name);
    } else {
      EmitDiag(AutocheckWarnings::unusedFunction, Info.getLocation());
    }
    return;
  }
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

    Diags.Clear();
    AutocheckDiagnostic::reportWarning(
        Diags, Info.getLocation(), AutocheckWarnings::unusedParameter, Name);

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
  case clang::diag::note_throw_in_dtor:
  case clang::diag::note_throw_in_function:
    // This should only be emitted after throw in noexcept warning.
    if (AutocheckDiagnostic::getLatestWarning() !=
        AutocheckWarnings::throwInNoexceptFunc)
      return;
    break;
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
  case clang::diag::warn_deprecated_register:
  case clang::diag::ext_register_storage_class:
    EmitDiag(AutocheckWarnings::registerKeywordUsed, Info.getLocation());
    return;
  case clang::diag::warn_ptr_arith_precedes_bounds:
  case clang::diag::warn_ptr_arith_exceeds_bounds:
  case clang::diag::warn_array_index_precedes_bounds:
  case clang::diag::warn_array_index_exceeds_bounds:
  case clang::diag::warn_ptr_arith_exceeds_max_addressable_bounds:
  case clang::diag::warn_array_index_exceeds_max_addressable_bounds:
  case clang::diag::warn_static_array_too_small: {
    // Save built-in diagnostic to reemit as a note.
    llvm::SmallVector<char> Message;
    Info.FormatDiagnostic(Message);

    EmitDiag(AutocheckWarnings::arrayBounds, Info.getLocation());

    Diags.Clear();
    AutocheckDiagnostic::reportWarning(Diags, Info.getLocation(),
                                       AutocheckWarnings::reemitNote,
                                       Message.data());

    return;
  }
  case clang::diag::note_array_declared_here: {
    llvm::SmallVector<char> Message;
    Info.FormatDiagnostic(Message);
    Diags.Clear();
    AutocheckDiagnostic::reportWarning(Diags, Info.getLocation(),
                                       AutocheckWarnings::reemitNote,
                                       Message.data());
    return;
  }
  case clang::diag::warn_noreturn_function_has_return_expr:
  // warn_falloff_noreturn_function is also emmited for -Wreturn-type where is
  // should be ignored.
  // TODO: Add getter for type of the last emitted warning to check when
  // emitting a note.
  case clang::diag::warn_falloff_noreturn_function:
    if (AutocheckContext::Get().isEnabled(AutocheckWarnings::invalidNoreturn))
      EmitDiag(AutocheckWarnings::invalidNoreturn, Info.getLocation());
    return;
  case clang::diag::ext_delete_void_ptr_operand:
  case clang::diag::warn_delete_incomplete:
    EmitDiag(AutocheckWarnings::deleteIncomplete, Info.getLocation());
    return;
  case clang::diag::note_forward_declaration:
    // Allow note for -Wdelete-incomplete.
    break;
  case clang::diag::warn_bool_switch_condition:
    EmitDiag(AutocheckWarnings::switchBool, Info.getLocation());
    return;
  case clang::diag::ext_deprecated_string_literal_conversion:
    EmitDiag(AutocheckWarnings::writableString, Info.getLocation());
    return;
  case clang::diag::warn_pp_undef_identifier:
    EmitDiag(AutocheckWarnings::undefMacroUsed, Info.getLocation());
    return;
  }

  Client->HandleDiagnostic(DiagLevel, Info);
}

} // namespace autocheck
