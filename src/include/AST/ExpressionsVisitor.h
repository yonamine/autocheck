//===--- ExpressionsVisitor.h - Visitor for expression related checks -----===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file performs checks for rules related to expressions by traversing the
// clang AST.
//
// This implements the following checks:
// - [M5-0-20] Non-constant operands to a binary bitwise operator shall have the
//             same underlying type.
// - [A5-1-2]  Variables shall not be implicitly captured in a lambda
//             expression.
// - [A5-1-6]  Return type of a non-void return type lambda expression should be
//             explicitly specified.
// - [A5-1-8]  Lambda expressions should not be defined inside another lambda
//             expression.
// - [M5-2-10] The increment (++) and decrement (--) operators shall not be
//             mixed with other operators in an expression.
// - [A5-16-1] The ternary conditional operator shall not be used as a
//             sub-expression.
// - [M6-2-1]  Assignment operators shall not be used in sub-expressions.
// - [M17-0-5] The setjmp macro and the longjmp function shall not be used.
// - [M18-0-3] The library functions abort, exit, getenv and system from library
//             <cstdlib> shall not be used.
// - [M18-0-5] The unbounded functions of library <cstring> shall not be used.
// - [A18-5-1] Functions malloc, calloc, realloc and free shall not be used.
// - [A18-9-1] The std::bind shall not be used.
// - [A18-9-3] The std::move shall not be used on objects declared const or
//             const&.
// - [A21-8-1] Arguments to character-handling functions shall be representable
//             as an unsigned char.
// - [A26-5-1] Pseudorandom numbers shall not be generated using std::rand().
//
//===----------------------------------------------------------------------===//

#ifndef AST_EXPRESSIONS_VISITOR_H
#define AST_EXPRESSIONS_VISITOR_H

#include "AutocheckContext.h"
#include "Diagnostics/AutocheckWarnings.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "llvm/ADT/SmallSet.h"
#include <forward_list>
#include <stack>

namespace autocheck {

/// Common interface for all expression related visitors.
class ExpressionsVisitorInterface {
public:
  virtual ~ExpressionsVisitorInterface();

  /// Custom traversal function called for each Stmt before it and its subtree
  /// are visited.
  virtual bool PreTraverseStmt(clang::Stmt *S);
  /// Custom traversal function called for each Stmt after it and its entire
  /// subtree have been visited.
  virtual bool PostTraverseStmt(clang::Stmt *S);

  virtual bool VisitLambdaExpr(const clang::LambdaExpr *LE);
  virtual bool VisitUnaryOperator(const clang::UnaryOperator *UO);
  virtual bool VisitConditionalOperator(const clang::ConditionalOperator *CO);
  virtual bool VisitBinaryOperator(const clang::BinaryOperator *BO);
  virtual bool VisitReturnStmt(const clang::ReturnStmt *RS);
  virtual bool VisitCallExpr(const clang::CallExpr *CE);
};

/// [A5-1-2] Variables shall not be implicitly captured in a lambda expression.
class ImplicitlyCapturedVarVisitor : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit ImplicitlyCapturedVarVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitLambdaExpr(const clang::LambdaExpr *LE) override;
};

/// [M5-2-10] The increment (++) and decrement (--) operators shall not be mixed
/// with other operators in an expression.
class IncDecOpMixedVisitor : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &ASTCtx;

public:
  explicit IncDecOpMixedVisitor(clang::DiagnosticsEngine &DE,
                                clang::ASTContext &ASTCtx);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitUnaryOperator(const clang::UnaryOperator *UO) override;

  bool getIncrementDecrementOpParent(const clang::Expr *E);
};

/// [A5-16-1] The ternary conditional operator shall not be used as a
/// sub-expression.
class TernaryOpSubExprVisitor : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &ASTCtx;

public:
  explicit TernaryOpSubExprVisitor(clang::DiagnosticsEngine &DE,
                                   clang::ASTContext &ASTCtx);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitConditionalOperator(const clang::ConditionalOperator *CO) override;

private:
  const clang::Stmt *getTernaryOpParent(const clang::Stmt *S);
};

/// [M6-2-1] Assignment operators shall not be used in sub-expressions.
class AssignmentOpSubExprVisitor : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &AC;

public:
  explicit AssignmentOpSubExprVisitor(clang::DiagnosticsEngine &DE,
                                      clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitBinaryOperator(const clang::BinaryOperator *BO) override;

private:
  bool getAssignmentOpParent(const clang::Expr *E) const;
};

/// [A5-1-8] Lambda expressions should not be defined inside another lambda
/// expression.
class NestedLambdaVisitor : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  unsigned LambdaCounter;

public:
  explicit NestedLambdaVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool PreTraverseStmt(clang::Stmt *S) override;
  bool PostTraverseStmt(clang::Stmt *S) override;
};

/// [A5-1-6] Return type of a non-void return type lambda expression should be
/// explicitly specified.
class LambdaReturnTypeVisitor : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  llvm::SmallVector<bool, 0> ReturnStmtSeenVector;

public:
  explicit LambdaReturnTypeVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool PreTraverseStmt(clang::Stmt *S) override;
  bool PostTraverseStmt(clang::Stmt *S) override;

  bool VisitReturnStmt(const clang::ReturnStmt *RS) override;
};

/// [A18-9-3] The std::move shall not be used on objects declared const or
/// const&.
class StdMoveUsedConstVisitor : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit StdMoveUsedConstVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCallExpr(const clang::CallExpr *CE) override;
};

/// [A21-8-1] Arguments to character-handling functions shall be representable
/// as an unsigned char.
class CharacterHandlingFunctionsVisitor : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::SourceManager &SM;

public:
  explicit CharacterHandlingFunctionsVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCallExpr(const clang::CallExpr *CE) override;
};

/// [A18-5-1] Functions malloc, calloc, realloc and free shall not be used.
///
/// [M18-0-3] The library functions abort, exit, getenv and system from library
/// <cstdlib> shall not be used.
///
/// [M18-0-5] The unbounded functions of library <cstring> shall not be used.
///
/// [M17-0-5] The setjmp macro and the longjmp function shall not be used.
///
/// [A18-9-1] The std::bind shall not be used.
///
/// [A26-5-1] Pseudorandom numbers shall not be generated using std::rand().
class StdFunctionUsed : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

  llvm::SmallSet<std::string, 4> StdlibMemFuncs;
  llvm::SmallSet<std::string, 4> StdlibFuncs;
  llvm::SmallSet<std::string, 11> StringFuncs;
  std::forward_list<bool (*)(clang::DiagnosticsEngine &, const std::string &,
                             clang::SourceLocation &, const std::string &)>
      AllFunctions;

public:
  explicit StdFunctionUsed(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCallExpr(const clang::CallExpr *CE) override;
};

/// [M5-0-20] Non-constant operands to a binary bitwise operator shall have the
/// same underlying type.
class ImplicitBitwiseBinOpConversionVisitor
    : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &ASTCtx;

  llvm::SmallSet<clang::BinaryOperator::Opcode, 10> AllowedBitwiseBinaryOps;

public:
  explicit ImplicitBitwiseBinOpConversionVisitor(clang::DiagnosticsEngine &DE,
                                                 clang::ASTContext &ASTCtx);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitBinaryOperator(const clang::BinaryOperator *BO);
};

/// Main visitor for expression related checks. Makes an instance of every class
/// that implement a ExpressionsVisitorInterface if appropriate flag is found.
/// Runs all Expression Visitors with one AST traversal.
class ExpressionsVisitor
    : public clang::RecursiveASTVisitor<ExpressionsVisitor> {
  clang::DiagnosticsEngine &DE;

  std::forward_list<std::unique_ptr<ExpressionsVisitorInterface>> AllVisitors;

public:
  explicit ExpressionsVisitor(clang::DiagnosticsEngine &DE,
                              clang::ASTContext &AC);
  void run(clang::TranslationUnitDecl *TUD);

  bool TraverseDecl(clang::Decl *D);
  bool TraverseStmt(clang::Stmt *S);

  bool VisitLambdaExpr(const clang::LambdaExpr *LE);
  bool VisitUnaryOperator(const clang::UnaryOperator *UO);
  bool VisitConditionalOperator(const clang::ConditionalOperator *CO);
  bool VisitBinaryOperator(const clang::BinaryOperator *BO);
  bool VisitReturnStmt(const clang::ReturnStmt *RS);
  bool VisitCallExpr(const clang::CallExpr *CE);
};

} // namespace autocheck

#endif
