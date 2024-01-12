//===--- StatementsVisitor.h - Visitor for statement related checks -------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file performs checks for rules related to statements by traversing the
// clang AST.
//
// This implements the following checks:
// - [M6-3-1] The statement forming the body of a switch, while, do ... while or
//            for statement shall be a compound statement.
// - [M6-4-2] All if ... else if constructs shall be terminated with an else
//            clause.
// - [M6-4-6] The final clause of a switch statement shall be the
//            default-clause.
// - [A6-5-3] Do statements should not be used.
// - [M6-6-1] Any label referenced by a goto statement shall be declared in the
//            same block, or in a block enclosing the goto statement.
// - [M6-6-2] The goto statement shall jump to a label declared later in the
//            same function body.
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

class AutocheckDiagnostic;

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
  virtual bool VisitCXXForRangeStmt(const clang::CXXForRangeStmt *CFRS);
  virtual bool VisitDoStmt(const clang::DoStmt *DS);
  virtual bool VisitWhileStmt(const clang::WhileStmt *WS);
  virtual bool VisitSwitchStmt(const clang::SwitchStmt *SS);
};

/// [M6-4-2] All if ... else if constructs shall be terminated with an else
/// clause.
class IfElseIfTerminatedVisitor : public StatementsVisitorInterface {
  AutocheckDiagnostic &AD;
  clang::ASTContext &AC;

public:
  explicit IfElseIfTerminatedVisitor(AutocheckDiagnostic &AD,
                                     clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitIfStmt(const clang::IfStmt *IS) override;
};

/// [M6-6-1] Any label referenced by a goto statement shall be declared in the
/// same block, or in a block enclosing the goto statement.
class GotoLabelBlockVisitor : public StatementsVisitorInterface {
  AutocheckDiagnostic &AD;
  clang::ASTContext &AC;

  const clang::Stmt *getGotoParent(const clang::Stmt *S,
                                   const clang::LabelStmt *LSWanted) const;

public:
  explicit GotoLabelBlockVisitor(AutocheckDiagnostic &AD,
                                 clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitGotoStmt(const clang::GotoStmt *GS) override;
};

/// [M6-4-6] The final clause of a switch statement shall be the default-clause.
class SwitchStmtDefaultClause : public StatementsVisitorInterface {
  AutocheckDiagnostic &AD;
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
  explicit SwitchStmtDefaultClause(AutocheckDiagnostic &AD,
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
  AutocheckDiagnostic &AD;
  clang::ASTContext &AC;
  llvm::SmallSet<clang::FullSourceLoc, 0> DoWhileMacros;

public:
  explicit DoWhileUsedVisitor(
      AutocheckDiagnostic &AD, clang::ASTContext &AC,
      const llvm::SmallSet<clang::SourceLocation, 0> &DoWhileMacroLocations);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitDoStmt(const clang::DoStmt *DS) override;
};

/// [M6-3-1] The statement forming the body of a switch, while, do ... while or
/// for statement shall be a compound statement.
class BodyCompoundStmtVisitor : public StatementsVisitorInterface {
  AutocheckDiagnostic &AD;

public:
  explicit BodyCompoundStmtVisitor(AutocheckDiagnostic &AD);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitDoStmt(const clang::DoStmt *DS) override;
  bool VisitWhileStmt(const clang::WhileStmt *WS) override;
  bool VisitSwitchStmt(const clang::SwitchStmt *SS) override;
  bool VisitForStmt(const clang::ForStmt *FS) override;
  bool VisitCXXForRangeStmt(const clang::CXXForRangeStmt *CFRS) override;
};

/// [M6-6-2] The goto statement shall jump to a label declared later in the same
/// function body.
class GotoBackJumpVisitor : public StatementsVisitorInterface {
  AutocheckDiagnostic &AD;

public:
  explicit GotoBackJumpVisitor(AutocheckDiagnostic &AD);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitGotoStmt(const clang::GotoStmt *GS) override;
};

/// Main visitor for AutosarStatements. Makes an instance of every class that
/// implement a StatementsVisitorInterface if appropriate flag is found. Runs
/// all Statement Visitors with one AST traversal.
class StatementsVisitor : public clang::RecursiveASTVisitor<StatementsVisitor> {
  AutocheckDiagnostic &AD;

  std::forward_list<std::unique_ptr<StatementsVisitorInterface>> AllVisitors;

public:
  explicit StatementsVisitor(AutocheckDiagnostic &AD, clang::ASTContext &AC,
                             const AutocheckPPCallbacks &Callbacks);
  void run(clang::TranslationUnitDecl *TUD);

  bool TraverseDecl(clang::Decl *D);
  bool TraverseStmt(clang::Stmt *S);

  bool VisitDefaultStmt(const clang::DefaultStmt *DS);
  bool VisitCaseStmt(const clang::CaseStmt *CS);
  bool VisitGotoStmt(const clang::GotoStmt *GS);
  bool VisitIfStmt(const clang::IfStmt *IS);
  bool VisitForStmt(const clang::ForStmt *FS);
  bool VisitCXXForRangeStmt(const clang::CXXForRangeStmt *CFRS);
  bool VisitDoStmt(const clang::DoStmt *DS);
  bool VisitWhileStmt(const clang::WhileStmt *WS);
  bool VisitSwitchStmt(const clang::SwitchStmt *SS);
};

} // namespace autocheck

#endif
