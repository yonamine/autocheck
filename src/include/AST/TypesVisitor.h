//===--- TypesVisitor.h - Visitor for type related checks -----------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file performs checks for rules related to types by traversing the clang
// AST.
//
// This implements the following checks:
// - [A2-13-3] Type wchar_t shall not be used.
// - [A5-0-2]  The condition of an if-statement and the condition of an
//             iteration statement shall have type bool.
// - [M5-0-11] The plain char type shall only be used for the storage and use of
//             character values.
// - [M5-0-12] Signed char and unsigned char type shall only be used for the
//             storage and use of numeric values.
// - [A18-1-1] C-style arrays shall not be used.
// - [A18-1-2] The std::vector<bool> specialization shall not be used.
//
//===----------------------------------------------------------------------===//

#ifndef AST_TYPES_VISITOR_H
#define AST_TYPES_VISITOR_H

#include "AutocheckContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/ADT/SmallSet.h"
#include <forward_list>

namespace autocheck {

/// Common interface for all autocheck type visitors.
class TypesVisitorInterface {
public:
  virtual ~TypesVisitorInterface();

  virtual bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *E);
  virtual bool VisitIfStmt(clang::IfStmt *IS);
  virtual bool VisitWhileStmt(clang::WhileStmt *WS);
  virtual bool VisitForStmt(clang::ForStmt *FS);
  virtual bool VisitDoStmt(clang::DoStmt *DS);
  virtual bool VisitTypeLoc(const clang::TypeLoc &TL);
  virtual bool VisitVarDecl(const clang::VarDecl *VD);
  virtual bool VisitCXXNewExpr(const clang::CXXNewExpr *NE);
};

/// [M5-0-11] The plain char type shall only be used for the storage and use of
/// character values.
class CharStorageVisitor : public TypesVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &ASTCtx;

public:
  explicit CharStorageVisitor(clang::DiagnosticsEngine &DE,
                              clang::ASTContext &ASTCtx);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *E);

private:
  bool isPlainChar(const clang::QualType &Type) const;
};

/// [M5-0-12] Signed char and unsigned char type shall only be used for the
/// storage and use of numeric values.
class SignCharStorageVisitor : public TypesVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &ASTCtx;

public:
  explicit SignCharStorageVisitor(clang::DiagnosticsEngine &DE,
                                  clang::ASTContext &ASTCtx);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *ICE);

private:
  bool isPlainChar(const clang::QualType &Type) const;
  bool isUnsignedOrSignedChar(const clang::QualType &Type);
};

/// [A5-0-2] The condition of an if-statement and the condition of an iteration
/// statement shall have type bool.
class ConditionNotBoolVisitor : public TypesVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::BeforeThanCompare<clang::SourceLocation> IsBefore;
  clang::SourceRange ConditionSourceRange;

public:
  explicit ConditionNotBoolVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitIfStmt(clang::IfStmt *IS);
  bool VisitWhileStmt(clang::WhileStmt *WS);
  bool VisitForStmt(clang::ForStmt *FS);
  bool VisitDoStmt(clang::DoStmt *DS);
  bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *ICE);

private:
  /// Traverse the condition part and try to find IntegralToBoolean cast.
  void diagnoseCondition(clang::Expr *Condition);
};

/// [A2-13-3] Type wchar_t shall not be used.
class TypeWchartVisitor : public TypesVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &AC;

public:
  explicit TypeWchartVisitor(clang::DiagnosticsEngine &DE,
                             clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitTypeLoc(const clang::TypeLoc &TL) override;
};

/// [A18-1-1] C-style arrays shall not be used.
class CStyleArrayVisitor : public TypesVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &AC;
  /// TypeLoc of the last constexpr declared variable which should be exempt
  /// from the rule.
  clang::TypeLoc IgnoredTypeLoc;

public:
  explicit CStyleArrayVisitor(clang::DiagnosticsEngine &DE,
                              clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitVarDecl(const clang::VarDecl *VD) override;
  bool VisitCXXNewExpr(const clang::CXXNewExpr *NE) override;
  bool VisitTypeLoc(const clang::TypeLoc &TL) override;
};

/// [A18-1-2] The std::vector<bool> specialization shall not be used.
class BoolVectorUsedVisitor : public TypesVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &AC;

public:
  explicit BoolVectorUsedVisitor(clang::DiagnosticsEngine &DE,
                                 clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitTypeLoc(const clang::TypeLoc &TL) override;
};

/// Main visitor for type related checks. Makes an instance of every class that
/// implement a TypesVisitorInterface if appropriate flag is found. Runs all
/// Type Visitors with one AST traversal.
class TypesVisitor : public clang::RecursiveASTVisitor<TypesVisitor> {
  clang::DiagnosticsEngine &DE;

  std::forward_list<std::unique_ptr<TypesVisitorInterface>> AllVisitors;

public:
  explicit TypesVisitor(clang::DiagnosticsEngine &DE, clang::ASTContext &AC);
  void run(clang::TranslationUnitDecl *TUD);

  bool TraverseDecl(clang::Decl *D);

  bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *E);
  bool VisitIfStmt(clang::IfStmt *IS);
  bool VisitWhileStmt(clang::WhileStmt *WS);
  bool VisitForStmt(clang::ForStmt *FS);
  bool VisitDoStmt(clang::DoStmt *DS);
  bool VisitTypeLoc(const clang::TypeLoc &TL);
  bool VisitVarDecl(const clang::VarDecl *VD);
  bool VisitCXXNewExpr(const clang::CXXNewExpr *NE);
};

} // namespace autocheck

#endif
