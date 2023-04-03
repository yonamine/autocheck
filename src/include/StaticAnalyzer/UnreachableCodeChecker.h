//===--- UnreachableCodeChecker.h - Detector for unreachable code ---------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines UnreachableCodeChecker, that checks for unreachable code in
// a file.
//
//===----------------------------------------------------------------------===//

#ifndef STATIC_ANALYZER_UNREACHABLE_CODE_CHECKER_H
#define STATIC_ANALYZER_UNREACHABLE_CODE_CHECKER_H

#include "clang/Basic/Diagnostic.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ExplodedGraph.h"
#include "llvm/ADT/SmallSet.h"

namespace autocheck {

class UnreachableCodeChecker
    : public clang::ento::Checker<clang::ento::check::EndAnalysis> {
public:
  void checkEndAnalysis(clang::ento::ExplodedGraph &G,
                        clang::ento::BugReporter &B,
                        clang::ento::ExprEngine &Eng) const;

private:
  typedef llvm::SmallSet<unsigned, 32> CFGBlocksSet;

  static inline const clang::Stmt *
  getUnreachableStmt(const clang::CFGBlock *CB);
  /// Recursively finds the entry point(s) for this dead CFGBlock.
  static void FindUnreachableEntryPoints(const clang::CFGBlock *CB,
                                         CFGBlocksSet &Reachable,
                                         CFGBlocksSet &Visited);
  /// Determines if the path to this CFGBlock contained an element that infers
  /// this block is a false positive. We assume that FindUnreachableEntryPoints
  /// has already marked only the entry points to any dead code, so we need
  /// only to find the condition that led to this block (the predecessor of
  /// this block.) There will never be more than one predecessor.
  static bool isInvalidPath(const clang::CFGBlock *CB);
  /// Returns true if the given CFGBlock is empty.
  static inline bool isEmptyCFGBlock(const clang::CFGBlock *CB);
  /// Iterate over exploded graph and mark CFG blocks that correspond to some
  /// exploded graph node as reachable.
  void iterateOverExplodedGraph(clang::ento::ExplodedGraph &G,
                                llvm::DenseMap<const clang::LocationContext *,
                                               CFGBlocksSet> &Reachable) const;
  /// Check if CFG block is unreachable.
  bool isUnreachable(CFGBlocksSet &BSRachable, CFGBlocksSet &BSVisited,
                     const clang::CFGBlock *CB,
                     clang::ento::ExprEngine &Eng) const;
  /// Check if unreachable statement is do-while statement.
  bool isDoWhile(const clang::CFGBlock *CB,
                 const clang::LocationContext *LC) const;
  /// Check if unreachable statement is  __builtin_unreachable().
  bool isBuiltinUnreachable(const clang::CFGBlock *CB,
                            clang::ento::ExprEngine &Eng) const;
};

} // namespace autocheck

#endif
