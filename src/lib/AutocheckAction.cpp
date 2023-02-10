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

#include "AST/LexicalRulesVisitor.h"
#include "Diagnostics/AutocheckDiagnosticConsumer.h"
#include "Lex/AutocheckLex.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "llvm/Support/raw_ostream.h"

namespace autocheck {

AutocheckAction::AutocheckAction() : Context(AutocheckContext::Get()){};

bool AutocheckAction::usesPreprocessorOnly() const { return false; }

bool AutocheckAction::BeginInvocation(clang::CompilerInstance &CI) {
  AutocheckDiagnosticConsumer *DiagConsumer =
      new AutocheckDiagnosticConsumer(CI.getDiagnostics());
  CI.getDiagnostics().setClient(DiagConsumer, /*ShouldOwnClient=*/true);
  return true;
}

void AutocheckAction::ExecuteAction() {
  clang::CompilerInstance &CI = getCompilerInstance();
  if (!CI.hasPreprocessor())
    return;

  clang::Preprocessor &PP = getCompilerInstance().getPreprocessor();
  clang::SourceManager &SM = getCompilerInstance().getSourceManager();

  // Create and run lexer/preprocessor checks. Three types of checks are
  // performed:
  // 1. A raw lexer pass which check all rules without the preprocessor. These
  //    checks are run immediately on the entire file.
  // 2. A token handler with the preprocessor active. Thsese checks are
  //    performed in a callback function for each lexed token.
  // 3. Preprocessor checks. These are perfomed on certain preprocessor events.
  AutocheckLex LexerChecks(getCompilerInstance());
  LexerChecks.Run();

  if (!CI.hasSema())
    CI.createSema(getTranslationUnitKind(), /*CodeCompleteConsumer*/ nullptr);

  clang::ParseAST(CI.getSema(), CI.getFrontendOpts().ShowStats,
                  CI.getFrontendOpts().SkipFunctionBodies);
}

std::unique_ptr<clang::ASTConsumer>
AutocheckAction::CreateASTConsumer(clang::CompilerInstance &CI,
                                   llvm::StringRef InFile) {
  return std::make_unique<AutocheckASTConsumer>();
}

void AutocheckASTConsumer::HandleTranslationUnit(clang::ASTContext &ASTCtx) {
  clang::DiagnosticsEngine &DE = ASTCtx.getDiagnostics();
  clang::TranslationUnitDecl *TUD = ASTCtx.getTranslationUnitDecl();

  LexicalRulesVisitor(DE, ASTCtx).run(TUD);
}

} // namespace autocheck
