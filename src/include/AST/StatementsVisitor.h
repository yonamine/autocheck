//===--- StatementsVisitor.h - Visitor for statement related checks -------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file performs checks for rules related to statements by traversing the
// clang AST.
//
// This implements the following checks:
// - [M6-4-2] All if ... else if constructs shall be terminated with an else
//            clause.
// - [M6-4-6] The final clause of a switch statement shall be the
//            default-clause.
// - [A6-5-3] Do statements should not be used.
// - [M6-6-1] Any label referenced by a goto statement shall be declared in the
//            same block, or in a block enclosing the goto statement.
//
//===----------------------------------------------------------------------===//

#ifndef AST_STATEMENTS_VISITOR_H
#define AST_STATEMENTS_VISITOR_H

#include "Lex/AutocheckPPCallbacks.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "llvm/ADT/SmallSet.h"
#include <forward_list>

namespace autocheck {

/// Common interface for all autocheck statement visitors.
class StatementsVisitorInterface {
public:
  virtual ~StatementsVisitorInterface();

  /// Custom traversal function called for each Stmt before it and its subtree
  /// are visited.
  virtual bool PreTraverseStmt(clang::Stmt *S);
  /// Custom traversal function called for each Stmt after it and its entire
  /// subtree have been visited.
  virtual bool PostTraverseStmt(clang::Stmt *S);

  virtual bool VisitDefaultStmt(const clang::DefaultStmt *DS);
  virtual bool VisitCaseStmt(const clang::CaseStmt *CS);
  virtual bool VisitGotoStmt(const clang::GotoStmt *GS);
  virtual bool VisitIfStmt(const clang::IfStmt *IS);
  virtual bool VisitForStmt(const clang::ForStmt *FS);
  virtual bool VisitDoStmt(const clang::DoStmt *DS);
  virtual bool VisitWhileStmt(const clang::WhileStmt *WS);
};

/// [M6-4-2] All if ... else if constructs shall be terminated with an else
/// clause.
class IfElseIfTerminatedVisitor : public StatementsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  const AutocheckContext &Context;
  clang::ASTContext &AC;

public:
  explicit IfElseIfTerminatedVisitor(clang::DiagnosticsEngine &DE,
                                     const AutocheckContext &Context,
                                     clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitIfStmt(const clang::IfStmt *IS) override;
};

/// [M6-6-1] Any label referenced by a goto statement shall be declared in the
/// same block, or in a block enclosing the goto statement.
class GotoLabelBlockVisitor : public StatementsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  const AutocheckContext &Context;
  clang::ASTContext &AC;

  const clang::Stmt *getGotoParent(const clang::Stmt *S,
                                   const clang::LabelStmt *LSWanted);

public:
  explicit GotoLabelBlockVisitor(clang::DiagnosticsEngine &DE,
                                 const AutocheckContext &Context,
                                 clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitGotoStmt(const clang::GotoStmt *GS) override;
};

/// [M6-4-6] The final clause of a switch statement shall be the default-clause.
class SwitchStmtDefaultClause : public StatementsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  const AutocheckContext &Context;
  clang::ASTContext &AC;

  /// This structure holds data about a single switch statement.
  struct SwitchStmtData {
    const clang::SwitchStmt *SS; /// SwitchStmt node.
    bool DefaultStmtSeen;        /// Presence indicator for a default-clause.
    bool Valid;                  /// Validity indicator for a switch statement.

    SwitchStmtData(const clang::SwitchStmt *SS, bool DefaultStmtSeen,
                   bool Valid)
        : SS(SS), DefaultStmtSeen(DefaultStmtSeen), Valid(Valid) {}
  };
  llvm::SmallVector<SwitchStmtData, 0> SwitchStmtVector;
  llvm::SmallVector<bool, 0> SwitchStmtSeenVector;

public:
  explicit SwitchStmtDefaultClause(clang::DiagnosticsEngine &DE,
                                   const AutocheckContext &Context,
                                   clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool PreTraverseStmt(clang::Stmt *S) override;
  bool PostTraverseStmt(clang::Stmt *S) override;

  bool VisitDefaultStmt(const clang::DefaultStmt *DS) override;
  bool VisitCaseStmt(const clang::CaseStmt *CS) override;
};

/// [A6-5-3] Do statements should not be used.
class DoWhileUsedVisitor : public StatementsVisitorInterface {
  static bool IsPresentDoWhileFlag;
  clang::DiagnosticsEngine &DE;
  const AutocheckContext &Context;
  clang::ASTContext &AC;
  llvm::SmallSet<clang::FullSourceLoc, 0> DoWhileMacros;

public:
  explicit DoWhileUsedVisitor(
      clang::DiagnosticsEngine &DE, const AutocheckContext &Context,
      clang::ASTContext &AC,
      const llvm::SmallSet<clang::SourceLocation, 0> &DoWhileMacroLocations);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitDoStmt(const clang::DoStmt *DS) override;
};

/// Main visitor for AutosarStatements. Makes an instance of every class that
/// implement a StatementsVisitorInterface if appropriate flag is found. Runs
/// all Statement Visitors with one AST traversal.
class StatementsVisitor : public clang::RecursiveASTVisitor<StatementsVisitor> {
  clang::DiagnosticsEngine &DE;

  std::forward_list<std::unique_ptr<StatementsVisitorInterface>> AllVisitors;

public:
  explicit StatementsVisitor(clang::DiagnosticsEngine &DE,
                             clang::ASTContext &AC,
                             const AutocheckPPCallbacks &Callbacks);
  void run(clang::TranslationUnitDecl *TUD);

  bool TraverseDecl(clang::Decl *D);
  bool TraverseStmt(clang::Stmt *S);

  bool VisitDefaultStmt(const clang::DefaultStmt *DS);
  bool VisitCaseStmt(const clang::CaseStmt *CS);
  bool VisitGotoStmt(const clang::GotoStmt *GS);
  bool VisitIfStmt(const clang::IfStmt *IS);
  bool VisitForStmt(const clang::ForStmt *FS);
  bool VisitDoStmt(const clang::DoStmt *DS);
  bool VisitWhileStmt(const clang::WhileStmt *WS);
};

} // namespace autocheck

#endif
