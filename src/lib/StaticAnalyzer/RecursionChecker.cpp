//===--- RecursionChecker.cpp - Detector for recursive functions ----------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements RecursionChecker, that checks for a recursion in a file
// (direct or indirect).
//
//===----------------------------------------------------------------------===//

#include "StaticAnalyzer/RecursionChecker.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h"

namespace autocheck {

std::string
RecursionChecker::getFunctionName(const clang::CallGraphNode *Node) const {
  return Node->getDecl()
      ->getAsFunction()
      ->getNameInfo()
      .getName()
      .getAsString();
}

VectorOfNodes::const_iterator
RecursionChecker::findMin(const VectorOfNodes &FunctionsInCycle) const {
  auto MinIt = FunctionsInCycle.begin();
  std::string Min(getFunctionName(*MinIt));

  for (auto It = FunctionsInCycle.begin(); It != FunctionsInCycle.end(); It++) {
    if (getFunctionName(*It) < Min) {
      Min = getFunctionName(*It);
      MinIt = It;
    }
  }
  return MinIt;
}

std::string RecursionChecker::makeWarningString(
    const VectorOfNodes &FunctionsInCycle,
    VectorOfNodes::const_iterator &MinIt) const {
  std::string Functions;
  auto It = MinIt;
  It++;

  while (It != MinIt) {
    if (It == FunctionsInCycle.end()) {
      It = FunctionsInCycle.begin();
      continue;
    }
    Functions = getFunctionName(*It) + " " + Functions;
    It++;
  }
  Functions = "Recursion cycle: " + getFunctionName(*It) + " " + Functions +
              " [A5-6-1]";
  return Functions;
}

void RecursionChecker::reportBug(const clang::CallGraph &CG,
                                 const VectorOfNodes &FunctionsInCycle,
                                 clang::ento::AnalysisManager &mgr,
                                 clang::ento::BugReporter &BR) const {
  auto Min = findMin(FunctionsInCycle);
  std::string Functions = makeWarningString(FunctionsInCycle, Min);

  BT.reset(new clang::ento::BugType(this, Functions, "Autosar"));
  auto Report = std::make_unique<clang::ento::BasicBugReport>(
      *BT, BT->getDescription(),
      clang::ento::PathDiagnosticLocation((*Min)->getDecl(),
                                          mgr.getSourceManager()));

  BR.emitReport(std::move(Report));
}

void RecursionChecker::dfsCycle(
    VectorOfNodes &FunctionsInCycle, clang::CallGraphNode *Current,
    clang::CallGraphNode *Parent,
    llvm::DenseMap<clang::CallGraphNode *, int> &Color,
    llvm::DenseMap<clang::CallGraphNode *, clang::CallGraphNode *>
        &ChildrenParentMap,
    SetOfNodes &Visited) const {
  if (FunctionsInCycle.size() || Color[Current] == Black)
    return;

  if (Color[Current] == Gray) {

    clang::CallGraphNode *PreviousNode = Parent;

    FunctionsInCycle.push_back(PreviousNode);

    while (PreviousNode != Current) {
      PreviousNode = ChildrenParentMap[PreviousNode];
      FunctionsInCycle.push_back(PreviousNode);
    }
    return;
  }

  ChildrenParentMap[Current] = Parent;
  Visited.insert(Current);
  Color[Current] = Gray;

  for (auto Chld = Current->begin(); Chld != Current->end(); Chld++) {
    dfsCycle(FunctionsInCycle, *Chld, Current, Color, ChildrenParentMap,
             Visited);
  }
  Color[Current] = Black;
}

bool RecursionChecker::searchForCycles(const clang::CallGraph &CG,
                                       VectorOfNodes &FunctionsInCycle,
                                       const SetOfNodes &Nodes,
                                       SetOfNodes &Visited) const {
  for (auto I = Nodes.begin(); I != Nodes.end(); I++) {
    llvm::DenseMap<clang::CallGraphNode *, int> Color;
    llvm::DenseMap<clang::CallGraphNode *, clang::CallGraphNode *> Par;

    dfsCycle(FunctionsInCycle, *I, nullptr, Color, Par, Visited);

    if (FunctionsInCycle.size() == 0)
      continue;
    else
      return true;
  }
  return false;
}

void RecursionChecker::markNodesWithParents(
    const clang::CallGraphNode *I, SetOfNodes &NodesWithParents) const {
  for (auto Node = I->begin(); Node != I->end(); Node++) {
    if (NodesWithParents.find(*Node) != NodesWithParents.end())
      continue;
    NodesWithParents.insert(*Node);
    markNodesWithParents(*Node, NodesWithParents);
  }
}

void RecursionChecker::classifyNodes(const clang::CallGraph &CG,
                                     SetOfNodes &NodesWithoutParents,
                                     SetOfNodes &NodesWithParents) const {
  auto I = CG.begin();
  I++;
  for (; I != CG.end(); I++) {
    if (NodesWithParents.find(I->second.get()) == NodesWithParents.end())
      markNodesWithParents(I->second.get(), NodesWithParents);
  }
  I = CG.begin();
  I++;
  for (; I != CG.end(); I++)
    if (NodesWithParents.find(I->second.get()) == NodesWithParents.end())
      NodesWithoutParents.insert(I->second.get());
}

bool RecursionChecker::allNodesVisited(const clang::CallGraph &CG,
                                       const SetOfNodes &Visited) const {
  auto It = CG.begin();
  It++;
  for (; It != CG.end(); It++) {
    if (Visited.count(It->second.get()) == 0)
      return false;
  }
  return true;
}

bool RecursionChecker::findCycles(const clang::CallGraph &CG,
                                  VectorOfNodes &FunctionsInCycle) const {
  SetOfNodes Visited;
  SetOfNodes NodesWithoutParents;
  SetOfNodes NodesWithParents;

  classifyNodes(CG, NodesWithoutParents, NodesWithParents);

  if (searchForCycles(CG, FunctionsInCycle, NodesWithoutParents, Visited))
    return true;

  if (!allNodesVisited(CG, Visited)) {
    if (searchForCycles(CG, FunctionsInCycle, NodesWithParents, Visited))
      return true;
  }
  return false;
}

void RecursionChecker::checkASTDecl(const clang::TranslationUnitDecl *TU,
                                    clang::ento::AnalysisManager &mgr,
                                    clang::ento::BugReporter &BR) const {
  clang::CallGraph CG;
  CG.addToCallGraph(const_cast<clang::TranslationUnitDecl *>(TU));
  VectorOfNodes FunctionsInCycle;

  if (findCycles(CG, FunctionsInCycle))
    reportBug(CG, FunctionsInCycle, mgr, BR);
}

} // namespace autocheck
