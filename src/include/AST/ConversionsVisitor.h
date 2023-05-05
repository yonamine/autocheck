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
// - [M5-0-4]  An implicit integral conversion shall not change the signedness
//             of the underlying type.
// - [M5-0-5]  There shall be no implicit floating-integral conversions.
// - [M5-0-6]  An implicit integral or floating-point conversion shall not
//             reduce the size of the underlying type.
// - [M5-0-10] If the bitwise operators ~and << are applied to an operand with
//             an underlying type of unsigned char or unsigned short, the result
//             shall be immediately cast to the underlying type of the operand.
// - [A5-2-3]  A cast shall not remove any const or volatile qualification from
//             the type of a pointer or reference.
// - [M5-2-9]  A cast shall not convert a pointer type to an integral type.
// - [M5-2-12] An identifier with array type passed as a function argument shall
//             not decay to a pointer.
//
//===----------------------------------------------------------------------===//

#ifndef AST_CONVERSIONS_VISITOR_H
#define AST_CONVERSIONS_VISITOR_H

#include "AutocheckContext.h"
#include "Diagnostics/AutocheckDiagnostic.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "llvm/ADT/SmallSet.h"
#include <forward_list>

namespace autocheck {

/// Common interface for all conversion related visitors.
class ConversionsVisitorInterface {
public:
  virtual ~ConversionsVisitorInterface();

  virtual bool PreTraverseInitListExpr(const clang::InitListExpr *ILE);
  virtual bool PostTraverseInitListExpr(const clang::InitListExpr *ILE);
  virtual bool VisitCastExpr(const clang::CastExpr *E);
  virtual bool VisitBinaryOperator(const clang::BinaryOperator *BO);
  virtual bool VisitUnaryOperator(const clang::UnaryOperator *UO);
  virtual bool VisitCXXOperatorCallExpr(const clang::CXXOperatorCallExpr *E);
  virtual bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *E);
  virtual bool VisitCallExpr(const clang::CallExpr *CE);
  virtual bool VisitCXXConstructExpr(const clang::CXXConstructExpr *CCE);
  virtual bool VisitArraySubscriptExpr(const clang::ArraySubscriptExpr *ASE);
  virtual bool VisitVarDecl(const clang::VarDecl *VD);
  virtual bool VisitCXXConstCastExpr(const clang::CXXConstCastExpr *CE);
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

  bool VisitBinaryOperator(const clang::BinaryOperator *BO) override;
  bool VisitUnaryOperator(const clang::UnaryOperator *UO) override;

private:
  bool isExprBooleanType(const clang::Expr *E) const;
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

  bool VisitBinaryOperator(const clang::BinaryOperator *BO) override;
  bool VisitUnaryOperator(const clang::UnaryOperator *UO) override;
  bool VisitCXXOperatorCallExpr(const clang::CXXOperatorCallExpr *E) override;

private:
  bool isExprEnumerationType(const clang::Expr *E) const;
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

  bool VisitBinaryOperator(const clang::BinaryOperator *BO) override;
  bool VisitUnaryOperator(const clang::UnaryOperator *UO) override;

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

  bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *ICE) override;
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

  bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *ICE) override;

private:
  clang::SourceLocation sl;
  bool isReducedSizeFromSubExpr(const clang::ImplicitCastExpr *ICE) const;
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

  bool VisitUnaryOperator(const clang::UnaryOperator *UO) override;
  bool VisitBinaryOperator(const clang::BinaryOperator *BO) override;

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

  bool VisitCallExpr(const clang::CallExpr *CE) override;
  bool VisitCXXConstructExpr(const clang::CXXConstructExpr *CCE) override;
  bool VisitArraySubscriptExpr(const clang::ArraySubscriptExpr *ASE) override;
  bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *ICE) override;
  bool VisitVarDecl(const clang::VarDecl *VD) override;

private:
  /// Check if function argument contains ArrayToPointerDecay node.
  bool checkArgument(const clang::Expr *Arg) const;

  /// Check if array decays to a base class pointer.
  bool checkDecayToBaseClassPtr(const clang::ImplicitCastExpr *ICE) const;
};

/// [M4-10-1] NULL shall not be used as an integer value.
class NullToIntegerValueVisitor : public ConversionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit NullToIntegerValueVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCastExpr(const clang::CastExpr *CE) override;
};

/// [M4-10-2] Literal zero (0) shall not be used as the null-pointer-constant.
class ZeroToNullPointerVisitor : public ConversionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit ZeroToNullPointerVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCastExpr(const clang::CastExpr *CE) override;
};

/// [A4-10-1] Only nullptr literal shall be used as the null-pointer-constant.
class NullptrOnlyNullPtrConstVisitor : public ConversionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit NullptrOnlyNullPtrConstVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCastExpr(const clang::CastExpr *CE) override;
};

/// [M5-2-9] A cast shall not convert a pointer type to an integral type.
class CastPtrToIntegralVisitor : public ConversionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit CastPtrToIntegralVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCastExpr(const clang::CastExpr *CE) override;
};

/// [A5-2-3] A cast shall not remove any const or volatile qualification from
/// the type of a pointer or reference.
class CVDiscardCastVisitor : public ConversionsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit CVDiscardCastVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXConstCastExpr(const clang::CXXConstCastExpr *CCCE) override;
};

/// [M5-0-4] An implicit integral conversion shall not change the signedness of
/// the underlying type.
class ImpcastChangesSignednessVisitor : public ConversionsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  const clang::ASTContext &AC;
  llvm::SmallVector<bool, 4> IsInsideSyntacticILEForm;

public:
  explicit ImpcastChangesSignednessVisitor(clang::DiagnosticsEngine &DE,
                                           const clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool PreTraverseInitListExpr(const clang::InitListExpr *ILE) override;
  bool PostTraverseInitListExpr(const clang::InitListExpr *ILE) override;
  bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *ICE) override;
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
                              clang::ASTContext &AC);
  void run(clang::TranslationUnitDecl *TUD);
  bool TraverseDecl(clang::Decl *D);
  bool TraverseInitListExpr(clang::InitListExpr *ILE);
  bool shouldVisitImplicitCode() { return true; }
  bool VisitCastExpr(const clang::CastExpr *E);
  bool VisitBinaryOperator(const clang::BinaryOperator *BO);
  bool VisitUnaryOperator(const clang::UnaryOperator *UO);
  bool VisitCXXOperatorCallExpr(const clang::CXXOperatorCallExpr *COCE);
  bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *ICE);
  bool VisitCallExpr(const clang::CallExpr *CE);
  bool VisitCXXConstructExpr(const clang::CXXConstructExpr *CCE);
  bool VisitArraySubscriptExpr(const clang::ArraySubscriptExpr *ASE);
  bool VisitVarDecl(const clang::VarDecl *VD);
  bool VisitCXXConstCastExpr(const clang::CXXConstCastExpr *CCCE);
};

} // namespace autocheck

#endif
