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

#include "AST/ClassesVisitor.h"
#include "AST/ConversionsVisitor.h"
#include "AST/DeclarationsVisitor.h"
#include "AST/ExpressionsVisitor.h"
#include "AST/LexicalRulesVisitor.h"
#include "AST/LoopsVisitor.h"
#include "AST/StatementsVisitor.h"
#include "AST/TypesVisitor.h"
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

static void runVisitors(clang::ASTContext &ASTCtx,
                        const AutocheckPPCallbacks &Callbacks,
                        clang::Sema &SemaRef) {
  clang::DiagnosticsEngine &DE = ASTCtx.getDiagnostics();
  clang::TranslationUnitDecl *TUD = ASTCtx.getTranslationUnitDecl();

  LexicalRulesVisitor(DE, ASTCtx).run(TUD);
  StatementsVisitor(DE, ASTCtx, Callbacks).run(TUD);
  ClassesVisitor(DE, ASTCtx).run(TUD);
  DeclarationsVisitor(DE, ASTCtx, SemaRef).run(TUD);
  ExpressionsVisitor(DE, ASTCtx).run(TUD);
  ConversionsVisitor(DE, ASTCtx).run(TUD);
  TypesVisitor(DE, ASTCtx).run(TUD);
  ForLoopVisitor(DE, ASTCtx).run(TUD);
}

void AutocheckAction::ExecuteAction() {
  clang::CompilerInstance &CI = getCompilerInstance();
  if (!CI.hasPreprocessor())
    return;

  clang::Preprocessor &PP = getCompilerInstance().getPreprocessor();
  clang::SourceManager &SM = getCompilerInstance().getSourceManager();

  // Create and run lexer/preprocessor checks. Two types of checks are
  // performed:
  // 1. A raw lexer pass which check all rules without the preprocessor. These
  //    checks are run immediately on the entire file.
  // 2. A token handler with the preprocessor active. Thsese checks are
  //    performed in a callback function for each lexed token.
  AutocheckLex LexerChecks(getCompilerInstance());
  LexerChecks.Run();

  // Set up preprocessor callbacks.
  auto Callbacks = std::make_unique<AutocheckPPCallbacks>(CI.getDiagnostics());
  PPCallbacks = Callbacks.get();
  CI.getPreprocessor().addPPCallbacks(std::move(Callbacks));

  if (!CI.hasSema())
    CI.createSema(getTranslationUnitKind(), /*CodeCompleteConsumer*/ nullptr);

  clang::ParseAST(CI.getSema(), CI.getFrontendOpts().ShowStats,
                  CI.getFrontendOpts().SkipFunctionBodies);

  runVisitors(CI.getASTContext(), *PPCallbacks, CI.getSema());
}

std::unique_ptr<clang::ASTConsumer>
AutocheckAction::CreateASTConsumer(clang::CompilerInstance &CI,
                                   llvm::StringRef InFile) {
  return std::make_unique<AutocheckASTConsumer>();
}

void AutocheckASTConsumer::HandleTranslationUnit(clang::ASTContext &ASTCtx) {
  // NO-OP
  // All checks are performed in AutocheckAction::ExecuteAction after ParseAST
  // because the visitors require references to AutocheckPPCallbacks and Sema
  // which are not available at the time of creating this consumer.
}

} // namespace autocheck
