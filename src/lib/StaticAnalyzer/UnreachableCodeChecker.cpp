//===--- UnreachableCodeChecker.cpp - Detector for unreachable code -------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file implements UnreachableCodeChecker, that checks for unreachable code
// in a file.
//
//===----------------------------------------------------------------------===//

#include "StaticAnalyzer/UnreachableCodeChecker.h"

#include "Diagnostics/AutocheckDiagnostic.h"
#include "clang/AST/ParentMap.h"
#include "clang/Basic/Builtins.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerHelpers.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ExprEngine.h"

namespace autocheck {

void UnreachableCodeChecker::reportBug(clang::ento::BugReporter &C,
                                       clang::SourceLocation Loc) const {
  const auto &DiagInfo =
      AutocheckDiagnostic::GetDiagInfo(AutocheckWarnings::unreachableCode);
  if (!BT)
    BT.reset(
        new clang::ento::BuiltinBug(this, DiagInfo.Rule, DiagInfo.Message));

  auto R = std::make_unique<clang::ento::BasicBugReport>(
      *BT, DiagInfo.Message,
      clang::ento::PathDiagnosticLocation(Loc, C.getSourceManager()));
  C.emitReport(std::move(R));
}

void UnreachableCodeChecker::iterateOverExplodedGraph(
    clang::ento::ExplodedGraph &G,
    llvm::DenseMap<const clang::LocationContext *, CFGBlocksSet> &Reachable)
    const {
  for (clang::ento::ExplodedGraph::node_iterator I = G.nodes_begin();
       I != G.nodes_end(); ++I) {
    const clang::ProgramPoint &P = I->getLocation();
    const clang::LocationContext *LC = P.getLocationContext();
    const clang::Decl *D = LC->getAnalysisDeclContext()->getDecl();

    // Don't do anything for template instantiations.
    if (const clang::FunctionDecl *FD = llvm::dyn_cast<clang::FunctionDecl>(D))
      if (FD->isTemplateInstantiation())
        continue;

    if (llvm::Optional<clang::BlockEntrance> BE =
            P.getAs<clang::BlockEntrance>()) {
      const clang::CFGBlock *CB = BE->getBlock();
      Reachable[LC].insert(CB->getBlockID());
    }
  }
}

bool UnreachableCodeChecker::isUnreachable(CFGBlocksSet &BSReachable,
                                           CFGBlocksSet &BSVisited,
                                           const clang::CFGBlock *CB,
                                           clang::ento::ExprEngine &Eng) const {
  // Check if the block is unreachable.
  if (BSReachable.count(CB->getBlockID()))
    return false;

  // Check if the block is empty (an artificial block).
  if (isEmptyCFGBlock(CB))
    return false;

  // Find the entry points for this block.
  if (!BSVisited.count(CB->getBlockID()))
    FindUnreachableEntryPoints(CB, BSReachable, BSVisited);

  // This block may have been pruned; check if we still want to report it.
  if (BSReachable.count(CB->getBlockID()))
    return false;

  // Check for false positives
  if (isInvalidPath(CB))
    return false;

  // It is good practice to always have a "default" label in a "switch", even
  // if we should never get there. It can be used to detect errors, for
  // instance. Unreachable code directly under a "default" label is therefore
  // likely to be a false positive.
  if (const clang::Stmt *Label = CB->getLabel())
    if (Label->getStmtClass() == clang::Stmt::DefaultStmtClass)
      return false;

  // Special case for __builtin_unreachable.
  // FIXME: This should be extended to include other unreachable markers,
  // such as llvm_unreachable.
  if (!CB->empty())
    return !isBuiltinUnreachable(CB, Eng);

  return true;
}

void UnreachableCodeChecker::checkEndAnalysis(
    clang::ento::ExplodedGraph &G, clang::ento::BugReporter &B,
    clang::ento::ExprEngine &Eng) const {
  llvm::DenseMap<const clang::LocationContext *, CFGBlocksSet> Reachable;
  llvm::DenseMap<const clang::LocationContext *, CFGBlocksSet> Visited;

  // Iterate over ExplodedGraph.
  iterateOverExplodedGraph(G, Reachable);

  if (Reachable.empty())
    return;

  // Find CFGBlocks that were not covered by any node.
  for (auto i = Reachable.begin(); i != Reachable.end(); i++) {
    clang::CFG *CFGraph =
        (i->first)->getAnalysisDeclContext()->getUnoptimizedCFG();
    for (clang::CFG::const_iterator I = CFGraph->begin(), E = CFGraph->end();
         I != E; ++I) {
      const clang::CFGBlock *CB = *I;
      if (isUnreachable(Reachable[i->first], Visited[i->first], CB, Eng)) {
        if (const clang::Stmt *S = getUnreachableStmt(CB)) {
          clang::ento::PathDiagnosticLocation DL =
              clang::ento::PathDiagnosticLocation::createBegin(
                  S, B.getSourceManager(), i->first);
          clang::SourceLocation SL = DL.asLocation();
          if (SL.isValid() && !isDoWhile(CB, i->first))
            reportBug(B, SL);
        }
      }
    }
  }
}

void UnreachableCodeChecker::FindUnreachableEntryPoints(
    const clang::CFGBlock *CB, CFGBlocksSet &BSReachable,
    CFGBlocksSet &BSVisited) {
  BSVisited.insert(CB->getBlockID());

  for (clang::CFGBlock::const_pred_iterator I = CB->pred_begin(),
                                            E = CB->pred_end();
       I != E; ++I) {
    if (!*I)
      continue;

    if (!BSReachable.count((*I)->getBlockID())) {
      // If we find an unreachable predecessor, mark this block as reachable
      // so we don't report this block.
      BSReachable.insert(CB->getBlockID());
      if (!BSVisited.count((*I)->getBlockID()))
        // If we haven't previously visited the unreachable predecessor,
        // recurse.
        FindUnreachableEntryPoints(*I, BSReachable, BSVisited);
    }
  }
}

// Find the Stmt* in a CFGBlock for reporting a warning.
const clang::Stmt *
UnreachableCodeChecker::getUnreachableStmt(const clang::CFGBlock *CB) {
  for (clang::CFGBlock::const_iterator I = CB->begin(), E = CB->end(); I != E;
       ++I) {
    if (llvm::Optional<clang::CFGStmt> S = I->getAs<clang::CFGStmt>()) {
      if (!llvm::isa<clang::DeclStmt>(S->getStmt()))
        return S->getStmt();
    }
  }
  if (const clang::Stmt *S = CB->getTerminatorStmt())
    return S;
  else
    return nullptr;
}

bool UnreachableCodeChecker::isInvalidPath(const clang::CFGBlock *CB) {
  // We only expect a predecessor size of 0 or 1. If it is >1, then an
  // external condition has broken our assumption (for example, a sink being
  // placed by another check). In these cases, we choose not to report.
  if (CB->pred_size() > 1)
    return true;

  // If there are no predecessors, then this block is trivially unreachable.
  if (CB->pred_size() == 0)
    return false;

  const clang::CFGBlock *pred = *CB->pred_begin();
  if (!pred)
    return false;

  // Get the predecessor block's terminator conditon.
  const clang::Stmt *cond = pred->getTerminatorCondition();

  // assert(cond && "CFGBlock's predecessor has a terminator condition");
  // The previous assertion is invalid in some cases (eg do/while). Leaving
  // reporting of these situations on at the moment to help triage these
  // cases.
  if (!cond)
    return false;

  // Run each of the checks on the conditions.
  return clang::ento::containsMacro(cond) || clang::ento::containsEnum(cond) ||
         clang::ento::containsStaticLocal(cond) ||
         clang::ento::containsBuiltinOffsetOf(cond) ||
         clang::ento::containsStmt<clang::UnaryExprOrTypeTraitExpr>(cond);
}

bool UnreachableCodeChecker::isEmptyCFGBlock(const clang::CFGBlock *CB) {
  return CB->getLabel() == nullptr    // No labels
         && CB->size() == 0           // No statements
         && !CB->getTerminatorStmt(); // No terminator
}

bool UnreachableCodeChecker::isDoWhile(const clang::CFGBlock *CB,
                                       const clang::LocationContext *LC) const {
  if (const clang::Stmt *S = getUnreachableStmt(CB)) {
    //  In macros, 'do {...} while (0)' is often used. Don't warn about the
    //  condition 0 when it is unreachable.
    if (S->getBeginLoc().isMacroID())
      if (const auto *I = llvm::dyn_cast<clang::IntegerLiteral>(S))
        if (I->getValue() == 0ULL)
          if (const clang::Stmt *Parent = (&LC->getParentMap())->getParent(S))
            if (llvm::isa<clang::DoStmt>(Parent))
              return true;
  }
  return false;
}

bool UnreachableCodeChecker::isBuiltinUnreachable(
    const clang::CFGBlock *CB, clang::ento::ExprEngine &Eng) const {
  for (clang::CFGBlock::const_iterator ci = CB->begin(), ce = CB->end();
       ci != ce; ++ci) {
    if (llvm::Optional<clang::CFGStmt> S = (*ci).getAs<clang::CFGStmt>())
      if (const clang::CallExpr *CE =
              llvm::dyn_cast<clang::CallExpr>(S->getStmt())) {
        if (CE->getBuiltinCallee() == clang::Builtin::BI__builtin_unreachable ||
            CE->isBuiltinAssumeFalse(Eng.getContext())) {
          return true;
        }
      }
  }
  return false;
}

} // namespace autocheck
