//===--- ExpressionsVisitor.h - Visitor for expression related checks -----===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file performs checks for rules related to expressions by traversing the
// clang AST.
//
// This implements the following checks:
// - [M5-0-14] The first operand of a conditional-operator shall have type bool.
// - [M5-0-20] Non-constant operands to a binary bitwise operator shall have the
//             same underlying type.
// - [M5-0-21] Bitwise operators shall only be applied to operands of unsigned
//             underlying type.
// - [A5-1-2]  Variables shall not be implicitly captured in a lambda
//             expression.
// - [A5-1-6]  Return type of a non-void return type lambda expression should be
//             explicitly specified.
// - [A5-1-8]  Lambda expressions should not be defined inside another lambda
//             expression.
// - [M5-2-10] The increment (++) and decrement (--) operators shall not be
//             mixed with other operators in an expression.
// - [M5-3-1]  Each operand of the ! operator, the logical && or the logical ||
//             operators shall have type bool.
// - [M5-3-2]  The unary minus operator shall not be applied to an expression
//             whose underlying type is unsigned.
// - [M5-3-4]  Evaluation of the operand to the sizeof operator shall not
//             contain side effects.
// - [A5-16-1] The ternary conditional operator shall not be used as a
//             sub-expression.
// - [M5-18-1] The comma operator shall not be used.
// - [M6-2-1]  Assignment operators shall not be used in sub-expressions.
// - [A15-1-2] An exception object shall not be a pointer.
// - [M15-1-2] NULL shall not be thrown explicitly.
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
  virtual bool
  VisitUnaryExprOrTypeTraitExpr(const clang::UnaryExprOrTypeTraitExpr *UOTTE);
  virtual bool VisitCXXThrowExpr(const clang::CXXThrowExpr *TE);
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

private:
  bool getIncrementDecrementOpParent(const clang::Expr *E) const;
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
  const clang::Stmt *getTernaryOpParent(const clang::Stmt *S) const;
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

  bool VisitBinaryOperator(const clang::BinaryOperator *BO) override;
};

/// [M5-3-4] Evaluation of the operand to the sizeof operator shall not contain
/// side effects.
class SizeofSideEffectVisitor : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &AC;

public:
  explicit SizeofSideEffectVisitor(clang::DiagnosticsEngine &DE,
                                   clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitUnaryExprOrTypeTraitExpr(
      const clang::UnaryExprOrTypeTraitExpr *UOTTE) override;
};

/// [M5-0-14] The first operand of a conditional-operator shall have type bool.
class ConditionalOpVisitor : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit ConditionalOpVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitConditionalOperator(const clang::ConditionalOperator *CO) override;
};

/// [M5-3-1] Each operand of the ! operator, the logical && or the logical ||
/// operators shall have type bool.
class LogicalOpBoolOperandVisitor : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit LogicalOpBoolOperandVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitUnaryOperator(const clang::UnaryOperator *UO) override;
  bool VisitBinaryOperator(const clang::BinaryOperator *BO) override;
};

/// [M5-3-2] The unary minus operator shall not be applied to an expression
/// whose underlying type is unsigned.
class UnaryMinusOnUnsignedVisitor : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit UnaryMinusOnUnsignedVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitUnaryOperator(const clang::UnaryOperator *UO) override;
};

/// [A15-1-2] An exception object shall not be a pointer.
class ExceptionObjectPtrVisitor : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit ExceptionObjectPtrVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXThrowExpr(const clang::CXXThrowExpr *TE) override;
};

/// [M15-1-2] NULL shall not be thrown explicitly.
class ExceptionObjectNullVisitor : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit ExceptionObjectNullVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXThrowExpr(const clang::CXXThrowExpr *TE) override;
};

/// [M5-0-21] Bitwise operators shall only be applied to operands of unsigned
/// underlying type.
class BitwiseUnsignedOperandsVisitor : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit BitwiseUnsignedOperandsVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitUnaryOperator(const clang::UnaryOperator *UO) override;
  bool VisitBinaryOperator(const clang::BinaryOperator *BO) override;
};

/// [M5-18-1] The comma operator shall not be used.
class CommaOperatorVisitor : public ExpressionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit CommaOperatorVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitBinaryOperator(const clang::BinaryOperator *BO) override;
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
  bool
  VisitUnaryExprOrTypeTraitExpr(const clang::UnaryExprOrTypeTraitExpr *UOTTE);
  bool VisitCXXThrowExpr(const clang::CXXThrowExpr *TE);
};

} // namespace autocheck

#endif
