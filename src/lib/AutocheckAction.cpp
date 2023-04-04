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
#include "AST/HeadersVisitor.h"
#include "AST/LexicalRulesVisitor.h"
#include "AST/LoopsVisitor.h"
#include "AST/Matchers.h"
#include "AST/StatementsVisitor.h"
#include "AST/TemplatesVisitor.h"
#include "AST/TypesVisitor.h"
#include "Diagnostics/AutocheckDiagnosticConsumer.h"
#include "Lex/AutocheckLex.h"
#include "StaticAnalyzer/DivZeroChecker.h"
#include "StaticAnalyzer/RecursionChecker.h"
#include "StaticAnalyzer/UnreachableCodeChecker.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "clang/StaticAnalyzer/Frontend/AnalysisConsumer.h"
#include "clang/StaticAnalyzer/Frontend/CheckerRegistry.h"
#include "llvm/ADT/SmallVector.h"
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
                        AutocheckPPCallbacks &Callbacks, clang::Sema &SemaRef) {
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
  HeadersVisitor(DE, Callbacks).run(TUD);
  TemplatesVisitor(DE, ASTCtx).run(TUD);
}

static void runMatchers(clang::ASTContext &ASTCtx) {
  // Maximum capacity of AllCallbacks. Should be increased when a new matcher
  // is added.
  constexpr unsigned MatchNum = 2U;
  // Track all callbacks since they are only created if necessary and so they
  // can be properly destructed after the matchers are done.
  llvm::SmallVector<clang::ast_matchers::MatchFinder::MatchCallback *, MatchNum>
      AllCallbacks;

  clang::ast_matchers::MatchFinder MF;
  clang::DiagnosticsEngine &DE = ASTCtx.getDiagnostics();
  AutocheckContext &Context = AutocheckContext::Get();

  // Init all matchers.
  if (UnusedReturnMatcher::isFlagPresent(Context)) {
    UnusedReturnMatcher::Callback *Callback =
        new UnusedReturnMatcher::Callback(DE);
    MF.addMatcher(UnusedReturnMatcher::makeMatcher(), Callback);
    AllCallbacks.push_back(Callback);
  }

  if (SelfAssignmentMatcher::isFlagPresent(Context)) {
    SelfAssignmentMatcher::Callback *Callback =
        new SelfAssignmentMatcher::Callback(DE);
    MF.addMatcher(SelfAssignmentMatcher::makeMatcher(), Callback);
    AllCallbacks.push_back(Callback);
  }

  assert(AllCallbacks.size() <= MatchNum &&
         "Size should not exceed originaly set capacity of AllCallbacks\n");

  // Run all matchers on AST.
  if (!AllCallbacks.empty())
    MF.matchAST(ASTCtx);

  // Destroy all created callbacks.
  for (clang::ast_matchers::MatchFinder::MatchCallback *Callback : AllCallbacks)
    delete Callback;
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
  runMatchers(CI.getASTContext());
}

std::unique_ptr<clang::ASTConsumer>
AutocheckAction::CreateASTConsumer(clang::CompilerInstance &CI,
                                   llvm::StringRef InFile) {
  // Run static analysis checks:
  std::unique_ptr<clang::ento::AnalysisASTConsumer> AnalysisConsumer =
      clang::ento::CreateAnalysisConsumer(CI);
  AnalysisConsumer->AddCheckerRegistrationFn(
      [](clang::ento::CheckerRegistry &Registry) {
        Registry.addChecker<DivZeroChecker>("autosar.DivZeroChecker",
                                            "Check for division by zero", "");
        Registry.addChecker<RecursionChecker>("autosar.RecursionChecker",
                                              "Check for use of recursion", "");
        Registry.addChecker<UnreachableCodeChecker>(
            "autosar.UnreachableCodeChecker", "Check for unreachable code", "");
      });
  CI.getAnalyzerOpts()->CheckersAndPackages = {
      {"autosar.DivZeroChecker",
       Context.isEnabled(AutocheckWarnings::divByZero)},
      {"alpha.core.PointerSub",
       Context.isEnabled(AutocheckWarnings::pointerSub)},
      {"autosar.RecursionChecker",
       Context.isEnabled(AutocheckWarnings::recursionUsed)},
      {"autosar.UnreachableCodeChecker",
       Context.isEnabled(AutocheckWarnings::unreachableCode)},
      {"core.NullDereference",
       Context.isEnabled(AutocheckWarnings::nullDereference)},
      {"cplusplus.NewDelete",
       Context.isEnabled(AutocheckWarnings::nullDereference)},
  };

  return std::move(AnalysisConsumer);
}

} // namespace autocheck
