//===--- StatementsVisitor.cpp - Visitor for statement related checks -----===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the StatementsVisitor class and all classes derived from
// StatementsVisitorInterface.
//
//===----------------------------------------------------------------------===//

#include "AST/StatementsVisitor.h"

#include "AutocheckContext.h"
#include "Diagnostics/AutocheckDiagnostic.h"
#include "clang/AST/ParentMapContext.h"

namespace autocheck {

/* StatementsVisitorInterface */

StatementsVisitorInterface::~StatementsVisitorInterface() {}

using SVI = StatementsVisitorInterface;
bool SVI::PreTraverseStmt(clang::Stmt *) { return true; }
bool SVI::PostTraverseStmt(clang::Stmt *) { return true; }
bool SVI::VisitDefaultStmt(const clang::DefaultStmt *) { return true; }
bool SVI::VisitCaseStmt(const clang::CaseStmt *) { return true; }
bool SVI::VisitGotoStmt(const clang::GotoStmt *) { return true; }
bool SVI::VisitIfStmt(const clang::IfStmt *) { return true; }
bool SVI::VisitForStmt(const clang::ForStmt *FS) { return true; }
bool SVI::VisitCXXForRangeStmt(const clang::CXXForRangeStmt *CFRS) {
  return true;
}
bool SVI::VisitDoStmt(const clang::DoStmt *DS) { return true; }
bool SVI::VisitWhileStmt(const clang::WhileStmt *WS) { return true; }
bool SVI::VisitSwitchStmt(const clang::SwitchStmt *SS) { return true; }

/* IfElseIfTerminatedVisitor */

IfElseIfTerminatedVisitor::IfElseIfTerminatedVisitor(
    clang::DiagnosticsEngine &DE, const AutocheckContext &Context,
    clang::ASTContext &AC)
    : DE(DE), Context(Context), AC(AC) {}

bool IfElseIfTerminatedVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::ifElseIfTerminated);
}

bool IfElseIfTerminatedVisitor::VisitIfStmt(const clang::IfStmt *IS) {
  const auto &Parents = AC.getParents(*IS);
  assert((Parents.size() > 0) && "At least one parent node expected.");

  if (Parents[0].get<clang::IfStmt>())
    return true;

  const clang::Stmt *ElseStmt = IS->getElse();
  if (!ElseStmt)
    return true;

  const clang::IfStmt *CurrentIfStmt =
      llvm::dyn_cast_if_present<clang::IfStmt>(ElseStmt);
  while (CurrentIfStmt) {
    ElseStmt = CurrentIfStmt->getElse();
    if (!ElseStmt) {
      if (AutocheckDiagnostic::reportWarning(
              DE, IS->getIfLoc(), AutocheckWarnings::ifElseIfTerminated)
              .limitReached())
        return false;
      break;
    }
    CurrentIfStmt = llvm::dyn_cast_if_present<clang::IfStmt>(ElseStmt);
  }

  return true;
}

/* GotoLabelBlockVisitor */

GotoLabelBlockVisitor::GotoLabelBlockVisitor(clang::DiagnosticsEngine &DE,
                                             const AutocheckContext &Context,
                                             clang::ASTContext &AC)
    : DE(DE), Context(Context), AC(AC) {}

bool GotoLabelBlockVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::gotoLabelBlock);
}

bool GotoLabelBlockVisitor::VisitGotoStmt(const clang::GotoStmt *GS) {
  if (!getGotoParent(GS, GS->getLabel()->getStmt())) {
    return !AutocheckDiagnostic::reportWarning(
                DE, GS->getGotoLoc(), AutocheckWarnings::gotoLabelBlock)
                .limitReached();
  }
  return true;
}

const clang::Stmt *
GotoLabelBlockVisitor::getGotoParent(const clang::Stmt *S,
                                     const clang::LabelStmt *LSWanted) {
  const auto &ParentIt = AC.getParents(*S).begin();
  if (ParentIt == AC.getParents(*S).end())
    return nullptr;

  if (const clang::Stmt *ParentStmt = ParentIt->get<const clang::Stmt>()) {
    const auto &EndIt = ParentStmt->child_end();
    for (auto It = ParentStmt->child_begin(); It != EndIt; ++It)
      if (const clang::LabelStmt *LS =
              llvm::dyn_cast_if_present<clang::LabelStmt>(*It))
        if (LS == LSWanted)
          return LS;

    return getGotoParent(ParentStmt, LSWanted);
  }

  return nullptr;
}

/* SwitchStmtDefaultClause */

SwitchStmtDefaultClause::SwitchStmtDefaultClause(
    clang::DiagnosticsEngine &DE, const AutocheckContext &Context,
    clang::ASTContext &AC)
    : DE(DE), Context(Context), AC(AC) {}

bool SwitchStmtDefaultClause::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::switchStmtDefaultClause);
}

bool SwitchStmtDefaultClause::VisitDefaultStmt(const clang::DefaultStmt *DS) {
  // A default-clause was seen.
  SwitchStmtVector.back().DefaultStmtSeen = true;

  return true;
}

bool SwitchStmtDefaultClause::VisitCaseStmt(const clang::CaseStmt *CS) {
  SwitchStmtData &SD = SwitchStmtVector.back();
  // If the current switch statement is valid and the default-clause was
  // seen before this case-clause, the switch statement becomes invalid.
  if (SD.Valid && SD.DefaultStmtSeen)
    SD.Valid = false;

  return true;
}

bool SwitchStmtDefaultClause::PreTraverseStmt(clang::Stmt *S) {
  SwitchStmtSeenVector.push_back(false);
  if (const clang::SwitchStmt *SS =
          llvm::dyn_cast_if_present<const clang::SwitchStmt>(S)) {
    SwitchStmtSeenVector.back() = true;
    // Add data about the current switch statement.
    SwitchStmtVector.push_back(SwitchStmtData(SS, false, true));
  }

  return true;
}

bool SwitchStmtDefaultClause::PostTraverseStmt(clang::Stmt *S) {
  if (SwitchStmtSeenVector.back()) {
    const SwitchStmtData &SD = SwitchStmtVector.back();
    // If the current switch statement is invalid or the default-clause
    // was not seen in the switch statement which does not cover all enum
    // cases, report a warning message.
    if (!SD.Valid || (!SD.SS->isAllEnumCasesCovered() && !SD.DefaultStmtSeen)) {
      if (AutocheckDiagnostic::reportWarning(
              DE, S->getBeginLoc(), AutocheckWarnings::switchStmtDefaultClause)
              .limitReached())
        return false;
    }

    // Remove data about the current switch statement.
    SwitchStmtVector.pop_back();
  }

  SwitchStmtSeenVector.pop_back();
  return true;
}

/* DoWhileUsedVisitor */

DoWhileUsedVisitor::DoWhileUsedVisitor(
    clang::DiagnosticsEngine &DE, const AutocheckContext &Context,
    clang::ASTContext &AC,
    const llvm::SmallSet<clang::SourceLocation, 0> &DoWhileMacroLocations)
    : DE(DE), Context(Context), AC(AC) {
  for (const clang::SourceLocation &Loc : DoWhileMacroLocations) {
    DoWhileMacros.insert(AC.getFullLoc(Loc).getSpellingLoc());
  }
}

bool DoWhileUsedVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::doWhileUsed);
}

bool DoWhileUsedVisitor::VisitDoStmt(const clang::DoStmt *DS) {
  for (const clang::FullSourceLoc &FSC : DoWhileMacros) {
    auto DSEndLoc = AC.getFullLoc(DS->getEndLoc()).getSpellingLoc();
    if (FSC == DSEndLoc)
      return true;
  }
  return !AutocheckDiagnostic::reportWarning(DE, DS->getBeginLoc(),
                                             AutocheckWarnings::doWhileUsed)
              .limitReached();
}

/* Implementation of BodyCompoundStmtVisitor */

BodyCompoundStmtVisitor::BodyCompoundStmtVisitor(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool BodyCompoundStmtVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::notCompoundStatement);
}

bool BodyCompoundStmtVisitor::VisitDoStmt(const clang::DoStmt *DS) {
  if (!llvm::dyn_cast_if_present<clang::CompoundStmt>(DS->getBody())) {
    return !AutocheckDiagnostic::reportWarning(
                DE, DS->getBeginLoc(), AutocheckWarnings::notCompoundStatement)
                .limitReached();
  }
  return true;
}

bool BodyCompoundStmtVisitor::VisitWhileStmt(const clang::WhileStmt *WS) {
  if (!llvm::dyn_cast_if_present<clang::CompoundStmt>(WS->getBody())) {
    return !AutocheckDiagnostic::reportWarning(
                DE, WS->getBeginLoc(), AutocheckWarnings::notCompoundStatement)
                .limitReached();
  }
  return true;
}

bool BodyCompoundStmtVisitor::VisitSwitchStmt(const clang::SwitchStmt *SS) {
  if (!llvm::dyn_cast_if_present<clang::CompoundStmt>(SS->getBody())) {
    return !AutocheckDiagnostic::reportWarning(
                DE, SS->getBeginLoc(), AutocheckWarnings::notCompoundStatement)
                .limitReached();
  }
  return true;
}

bool BodyCompoundStmtVisitor::VisitForStmt(const clang::ForStmt *FS) {
  if (!llvm::dyn_cast_if_present<clang::CompoundStmt>(FS->getBody())) {
    return !AutocheckDiagnostic::reportWarning(
                DE, FS->getBeginLoc(), AutocheckWarnings::notCompoundStatement)
                .limitReached();
  }
  return true;
}

bool BodyCompoundStmtVisitor::VisitCXXForRangeStmt(
    const clang::CXXForRangeStmt *CFRS) {
  if (!llvm::dyn_cast_if_present<clang::CompoundStmt>(CFRS->getBody())) {
    return !AutocheckDiagnostic::reportWarning(
                DE, CFRS->getBeginLoc(),
                AutocheckWarnings::notCompoundStatement)
                .limitReached();
  }
  return true;
}

/* Implementation of GotoBackJumpVisitor */

GotoBackJumpVisitor::GotoBackJumpVisitor(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool GotoBackJumpVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::gotoBackJump);
}

bool GotoBackJumpVisitor::VisitGotoStmt(const clang::GotoStmt *GS) {
  // A label is always in the same function as a goto pointing to it, so we only
  // need to check their relative locations.
  clang::BeforeThanCompare<clang::SourceLocation> isBefore(
      DE.getSourceManager());
  if (isBefore(GS->getLabel()->getLocation(), GS->getGotoLoc())) {
    return !AutocheckDiagnostic::reportWarning(DE, GS->getBeginLoc(),
                                               AutocheckWarnings::gotoBackJump)
                .limitReached();
  }

  return true;
}

/* StatementsVisitor */

StatementsVisitor::StatementsVisitor(clang::DiagnosticsEngine &DE,
                                     clang::ASTContext &AC,
                                     const AutocheckPPCallbacks &Callbacks)
    : DE(DE) {
  const AutocheckContext &Context = AutocheckContext::Get();
  if (IfElseIfTerminatedVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<IfElseIfTerminatedVisitor>(DE, Context, AC));
  if (GotoLabelBlockVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<GotoLabelBlockVisitor>(DE, Context, AC));
  if (SwitchStmtDefaultClause::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<SwitchStmtDefaultClause>(DE, Context, AC));
  if (DoWhileUsedVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<DoWhileUsedVisitor>(
        DE, Context, AC, Callbacks.getDoWhileMacros()));
  if (BodyCompoundStmtVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<BodyCompoundStmtVisitor>(DE));
  if (GotoBackJumpVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<GotoBackJumpVisitor>(DE));
}

void StatementsVisitor::run(clang::TranslationUnitDecl *TUD) {
  if (!AllVisitors.empty())
    this->TraverseDecl(TUD);
}

bool StatementsVisitor::TraverseDecl(clang::Decl *D) {
  if (!D) // Can be null if code had errors.
    return true;

  clang::SourceLocation Loc = D->getBeginLoc();

  if (Loc.isInvalid() || appropriateHeaderLocation(DE, Loc)) {
    RecursiveASTVisitor<StatementsVisitor>::TraverseDecl(D);
  }
  return true;
}

bool StatementsVisitor::TraverseStmt(clang::Stmt *S) {
  AllVisitors.remove_if([S](std::unique_ptr<StatementsVisitorInterface> &V) {
    return !V->PreTraverseStmt(S);
  });

  RecursiveASTVisitor<StatementsVisitor>::TraverseStmt(S);

  AllVisitors.remove_if([S](std::unique_ptr<StatementsVisitorInterface> &V) {
    return !V->PostTraverseStmt(S);
  });

  return true;
}

bool StatementsVisitor::VisitDefaultStmt(const clang::DefaultStmt *DS) {
  AllVisitors.remove_if([DS](std::unique_ptr<StatementsVisitorInterface> &V) {
    return !V->VisitDefaultStmt(DS);
  });
  return true;
}

bool StatementsVisitor::VisitCaseStmt(const clang::CaseStmt *CS) {
  AllVisitors.remove_if([CS](std::unique_ptr<StatementsVisitorInterface> &V) {
    return !V->VisitCaseStmt(CS);
  });
  return true;
}

bool StatementsVisitor::VisitGotoStmt(const clang::GotoStmt *GS) {
  AllVisitors.remove_if([GS](std::unique_ptr<StatementsVisitorInterface> &V) {
    return !V->VisitGotoStmt(GS);
  });
  return true;
}

bool StatementsVisitor::VisitIfStmt(const clang::IfStmt *IS) {
  AllVisitors.remove_if([IS](std::unique_ptr<StatementsVisitorInterface> &V) {
    return !V->VisitIfStmt(IS);
  });
  return true;
}

bool StatementsVisitor::VisitForStmt(const clang::ForStmt *FS) {
  AllVisitors.remove_if([FS](std::unique_ptr<StatementsVisitorInterface> &V) {
    return !V->VisitForStmt(FS);
  });
  return true;
}

bool StatementsVisitor::VisitCXXForRangeStmt(
    const clang::CXXForRangeStmt *CFRS) {
  AllVisitors.remove_if([CFRS](std::unique_ptr<StatementsVisitorInterface> &V) {
    return !V->VisitCXXForRangeStmt(CFRS);
  });
  return true;
}

bool StatementsVisitor::VisitDoStmt(const clang::DoStmt *DS) {
  AllVisitors.remove_if([DS](std::unique_ptr<StatementsVisitorInterface> &V) {
    return !V->VisitDoStmt(DS);
  });
  return true;
}

bool StatementsVisitor::VisitWhileStmt(const clang::WhileStmt *WS) {
  AllVisitors.remove_if([WS](std::unique_ptr<StatementsVisitorInterface> &V) {
    return !V->VisitWhileStmt(WS);
  });
  return true;
}

bool StatementsVisitor::VisitSwitchStmt(const clang::SwitchStmt *SS) {
  AllVisitors.remove_if([SS](std::unique_ptr<StatementsVisitorInterface> &V) {
    return !V->VisitSwitchStmt(SS);
  });
  return true;
}

} // namespace autocheck
