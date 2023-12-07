//===--- RecursionChecker.h - Detector for recursive functions ------------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file defines RecursionChecker, that checks for a recursion in a file
// (direct or indirect).
//
// [A7-5-2] Functions shall not call themselves, either directly or indirectly.
//
//===----------------------------------------------------------------------===//

#ifndef STATIC_ANALYZER_RECURSION_CHECKER_H
#define STATIC_ANALYZER_RECURSION_CHECKER_H

#include "clang/Analysis/CallGraph.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/SmallVector.h"

typedef llvm::SmallSet<clang::CallGraphNode *, 20> SetOfNodes;
typedef llvm::SmallVector<clang::CallGraphNode *, 20> VectorOfNodes;

namespace autocheck {
class RecursionChecker
    : public clang::ento::Checker<
          clang::ento::check::ASTDecl<clang::TranslationUnitDecl>> {

  mutable std::unique_ptr<clang::ento::BugType> BT;
  // Node is White if it is not visited.
  // Node is Gray if dfs from this node is in progress.
  // Node is Black if dfs from this node is finished.
  enum Color { White, Gray, Black };

  void dfsCycle(VectorOfNodes &FunctionsInCycle, clang::CallGraphNode *Current,
                clang::CallGraphNode *Parent,
                llvm::DenseMap<clang::CallGraphNode *, int> &Color,
                llvm::DenseMap<clang::CallGraphNode *, clang::CallGraphNode *>
                    &ChildrenParentMap,
                SetOfNodes &Visited) const;

  bool findCycles(const clang::CallGraph &CG,
                  VectorOfNodes &FunctionsInCycle) const;

  VectorOfNodes::const_iterator
  findMin(const VectorOfNodes &FunctionsInCycle) const;

  std::string makeWarningString(const VectorOfNodes &FunctionsInCycle,
                                VectorOfNodes::const_iterator &Min) const;

  void reportBug(const clang::CallGraph &CG,
                 const VectorOfNodes &FunctionsInCycle,
                 clang::ento::AnalysisManager &mgr,
                 clang::ento::BugReporter &BR) const;

  bool searchForCycles(const clang::CallGraph &CG,
                       VectorOfNodes &FunctionsInCycle, const SetOfNodes &Nodes,
                       SetOfNodes &Visited) const;

  void classifyNodes(const clang::CallGraph &CG,
                     SetOfNodes &NodesWithoutParents,
                     SetOfNodes &NodesWithParents) const;

  void markNodesWithParents(const clang::CallGraphNode *I,
                            SetOfNodes &NodesWithParents) const;

  bool allNodesVisited(const clang::CallGraph &CG,
                       const SetOfNodes &Visited) const;

  std::string getFunctionName(const clang::CallGraphNode *Node) const;

public:
  void checkASTDecl(const clang::TranslationUnitDecl *TU,
                    clang::ento::AnalysisManager &mgr,
                    clang::ento::BugReporter &BR) const;
};
} // namespace autocheck

#endif
