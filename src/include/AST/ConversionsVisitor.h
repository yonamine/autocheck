//===--- ConversionsVisitor.h - Visitor for conversion related checks -----===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file performs checks for rules related to conversions by traversing the
// clang AST.
//
// This implements the following checks:
// - [A4-5-1]  Expressions with type enum or enum class shall not be used as
//             operands to built-in and overloaded operators other than the
//             subscript operator [ ], the assignment operator =, the equality
//             operators == and !=, the unary & operator, and the relational
//             operators <, <=, >, >=.
// - [M4-5-1]  Expressions with type bool shall not be used as operands to
//             built-in operators other than the assignment operator =, the
//             logical operators &&, ||, !, the equality operators == and ! =,
//             the unary & operator, and the conditional operator.
// - [M4-5-3]  Expressions with type (plain) char and wchar_t shall not be used
//             as operands to built-in operators other than the assignment
//             operator =, the equality operators == and !=, and the unary &
//             operator.
// - [A4-10-1] Only nullptr literal shall be used as the null-pointer-constant.
// - [M4-10-1] NULL shall not be used as an integer value.
// - [M4-10-2] Literal zero (0) shall not be used as the null-pointer-constant.
// - [M5-0-5]  There shall be no implicit floating-integral conversions.
// - [M5-0-6]  An implicit integral or floating-point conversion shall not
//             reduce the size of the underlying type.
// - [M5-0-10] If the bitwise operators ~and << are applied to an operand with
//             an underlying type of unsigned char or unsigned short, the result
//             shall be immediately cast to the underlying type of the operand.
// - [M5-2-12] An identifier with array type passed as a function argument shall
//             not decay to a pointer.
//
//===----------------------------------------------------------------------===//

#ifndef AST_CONVERSIONS_VISITOR_H
#define AST_CONVERSIONS_VISITOR_H

#include "AutocheckContext.h"
#include "Diagnostics/AutocheckDiagnostic.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "llvm/ADT/SmallSet.h"
#include <forward_list>

namespace autocheck {

/// Common interface for all conversion related visitors.
class ConversionsVisitorInterface {
public:
  virtual ~ConversionsVisitorInterface();

  virtual bool VisitCastExpr(const clang::CastExpr *E);
  virtual bool VisitBinaryOperator(const clang::BinaryOperator *BO);
  virtual bool VisitUnaryOperator(const clang::UnaryOperator *UO);
  virtual bool VisitCXXOperatorCallExpr(const clang::CXXOperatorCallExpr *E);
  virtual bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *E);
  virtual bool VisitCallExpr(const clang::CallExpr *CE);
  virtual bool VisitCXXConstructExpr(const clang::CXXConstructExpr *CCE);
  virtual bool VisitArraySubscriptExpr(const clang::ArraySubscriptExpr *ASE);
  virtual bool VisitVarDecl(const clang::VarDecl *VD);
};

/// [M4-5-1] Expressions with type bool shall not be used as operands to
/// built-in operators other than the assignment operator =, the logical
/// operators &&, ||, !, the equality operators == and ! =, the unary &
/// operator, and the conditional operator.
class InvalidBoolExpressionVisitor : public ConversionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

  llvm::SmallSet<clang::UnaryOperator::Opcode, 2> AllowedUnaryOps;
  llvm::SmallSet<clang::BinaryOperator::Opcode, 5> AllowedBinaryOps;

public:
  explicit InvalidBoolExpressionVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitBinaryOperator(const clang::BinaryOperator *BO);
  bool VisitUnaryOperator(const clang::UnaryOperator *UO);

private:
  bool isExprBooleanType(const clang::Expr *E);
};

/// [A4-5-1] Expressions with type enum or enum class shall not be used as
/// operands to built-in and overloaded operators other than the subscript
/// operator [ ], the assignment operator =, the equality operators == and ! =,
/// the unary & operator, and the relational operators <, <=, >, >=.
class InvalidEnumExpressionVisitor : public ConversionsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  llvm::SmallSet<clang::UnaryOperator::Opcode, 1> AllowedUnaryOps;
  llvm::SmallSet<clang::BinaryOperator::Opcode, 7> AllowedBinaryOps;
  llvm::SmallSet<clang::OverloadedOperatorKind, 9> AllowedOperators;

public:
  explicit InvalidEnumExpressionVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitBinaryOperator(const clang::BinaryOperator *BO);
  bool VisitUnaryOperator(const clang::UnaryOperator *UO);
  bool VisitCXXOperatorCallExpr(const clang::CXXOperatorCallExpr *E);

private:
  bool isExprEnumerationType(const clang::Expr *E);
};

/// [M4-5-3] Expressions with type (plain) char and wchar_t shall not be used as
/// operands to built-in operators other than the assignment operator =, the
/// equality operators == and ! =, and the unary & operator.
class InvalidCharExpressionVisitor : public ConversionsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  llvm::SmallSet<clang::UnaryOperator::Opcode, 1> AllowedUnaryOps;
  llvm::SmallSet<clang::BinaryOperator::Opcode, 3> AllowedBinaryOps;
  llvm::SmallSet<clang::BinaryOperator::Opcode, 4> ExceptionOps;

public:
  explicit InvalidCharExpressionVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitBinaryOperator(const clang::BinaryOperator *BO);
  bool VisitUnaryOperator(const clang::UnaryOperator *UO);

private:
  bool isExprCharType(const clang::Expr *E) const;

  /// Check if given Expr is an IntegerLiteral in 0-9 range.
  bool isIntLessThan10(const clang::Expr *E) const;
};

/// [M5-0-5] There shall be no implicit floating-integral conversions.
class ImplicitFloatIntegralConversionVisitor
    : public ConversionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit ImplicitFloatIntegralConversionVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *E);
};

/// [M5-0-6] An implicit integral or floating-point conversion shall not reduce
/// the size of the underlying type.
class ImplicitSizeReductionConversionVisitor
    : public ConversionsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &ASTCtx;

public:
  explicit ImplicitSizeReductionConversionVisitor(clang::DiagnosticsEngine &DE,
                                                  clang::ASTContext &ASTCtx);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *E);

private:
  clang::SourceLocation sl;
  bool isReducedSizeFromSubExpr(const clang::ImplicitCastExpr *E) const;
};

/// [M5-0-10] If the bitwise operators ~and << are applied to an operand with an
/// underlying type of unsigned char or unsigned short, the result shall be
/// immediately cast to the underlying type of the operand.
class ImplicitBitwiseResultConversionVisitor
    : public ConversionsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &ASTCtx;

public:
  explicit ImplicitBitwiseResultConversionVisitor(clang::DiagnosticsEngine &DE,
                                                  clang::ASTContext &ASTCtx);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitUnaryOperator(const clang::UnaryOperator *UO);
  bool VisitBinaryOperator(const clang::BinaryOperator *BO);

private:
  bool isUnsignedCharOrShort(const clang::QualType &Type) const;
  bool isBeingExplicityCastToType(const clang::Expr *E,
                                  const clang::QualType &Type) const;
};

/// [M5-2-12] An identifier with array type passed as a function argument shall
/// not decay to a pointer.
class ArrayDecaysToPointerVisitor : public ConversionsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &ASTCtx;

public:
  explicit ArrayDecaysToPointerVisitor(clang::DiagnosticsEngine &DE,
                                       clang::ASTContext &ASTCtx);
  static bool isFlagPresent(const AutocheckContext &Context);

  /// Handles normal function calls, methods, overloaded operator calls.
  bool VisitCallExpr(const clang::CallExpr *CE);

  /// Handles constructor calls.
  bool VisitCXXConstructExpr(const clang::CXXConstructExpr *CCE);

  /// Handles array subscrip operations.
  bool VisitArraySubscriptExpr(const clang::ArraySubscriptExpr *ASE);

  /// Handles implicit casts.
  bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *E);

  /// Handles declarations.
  bool VisitVarDecl(const clang::VarDecl *VD);

private:
  /// Check if function argument contains ArrayToPointerDecay node.
  bool checkArgument(const clang::Expr *Arg);

  /// Check if array decays to a base class pointer.
  bool checkDecayToBaseClassPtr(const clang::ImplicitCastExpr *E);
};

/// [M4-10-1] NULL shall not be used as an integer value.
class NullToIntegerValueVisitor : public ConversionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit NullToIntegerValueVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCastExpr(const clang::CastExpr *E);
};

/// [M4-10-2] Literal zero (0) shall not be used as the null-pointer-constant.
class ZeroToNullPointerVisitor : public ConversionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit ZeroToNullPointerVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCastExpr(const clang::CastExpr *E);
};

/// [A4-10-1] Only nullptr literal shall be used as the null-pointer-constant.
class NullptrOnlyNullPtrConstVisitor : public ConversionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit NullptrOnlyNullPtrConstVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCastExpr(const clang::CastExpr *E);
};

/// Main visitor for conversion related checks. Makes an instance of every class
/// that implement a ConversionsVisitorInterface if appropriate flag is found.
/// Runs all Conversion Visitors with one AST traversal.
class ConversionsVisitor
    : public clang::RecursiveASTVisitor<ConversionsVisitor> {
  clang::DiagnosticsEngine &DE;

  std::forward_list<std::unique_ptr<ConversionsVisitorInterface>> AllVisitors;

public:
  explicit ConversionsVisitor(clang::DiagnosticsEngine &DE,
                              clang::ASTContext &ASTCtx);
  void run(clang::TranslationUnitDecl *TUD);
  bool TraverseDecl(clang::Decl *D);
  bool shouldVisitImplicitCode() { return true; }
  bool VisitCastExpr(const clang::CastExpr *E);
  bool VisitBinaryOperator(const clang::BinaryOperator *BO);
  bool VisitUnaryOperator(const clang::UnaryOperator *UO);
  bool VisitCXXOperatorCallExpr(const clang::CXXOperatorCallExpr *E);
  bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *E);
  bool VisitCallExpr(const clang::CallExpr *CE);
  bool VisitCXXConstructExpr(const clang::CXXConstructExpr *CCE);
  bool VisitArraySubscriptExpr(const clang::ArraySubscriptExpr *ASE);
  bool VisitVarDecl(const clang::VarDecl *VD);
};

} // namespace autocheck

#endif
