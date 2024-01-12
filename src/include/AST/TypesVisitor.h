//===--- TypesVisitor.h - Visitor for type related checks -----------------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file performs checks for rules related to types by traversing the clang
// AST.
//
// This implements the following checks:
// - [A0-4-2]  Type long double shall not be used.
// - [A2-13-3] Type wchar_t shall not be used.
// - [A5-0-2]  The condition of an if-statement and the condition of an
//             iteration statement shall have type bool.
// - [M5-0-11] The plain char type shall only be used for the storage and use of
//             character values.
// - [M5-0-12] Signed char and unsigned char type shall only be used for the
//             storage and use of numeric values.
// - [A5-1-7]  A lambda shall not be an operand to decltype or typeid.
// - [A18-1-1] C-style arrays shall not be used.
// - [A18-1-2] The std::vector<bool> specialization shall not be used.
// - [A18-1-3] The std::auto_ptr type shall not be used.
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

class AutocheckDiagnostic;

/// Common interface for all autocheck type visitors.
class TypesVisitorInterface {
public:
  virtual ~TypesVisitorInterface();

  virtual bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *ICE);
  virtual bool VisitIfStmt(clang::IfStmt *IS);
  virtual bool VisitWhileStmt(clang::WhileStmt *WS);
  virtual bool VisitForStmt(clang::ForStmt *FS);
  virtual bool VisitDoStmt(clang::DoStmt *DS);
  virtual bool VisitTypeLoc(const clang::TypeLoc &TL);
  virtual bool VisitVarDecl(const clang::VarDecl *VD);
  virtual bool VisitCXXNewExpr(const clang::CXXNewExpr *NE);
  virtual bool VisitCXXTypeidExpr(const clang::CXXTypeidExpr *CTE);
};

/// [M5-0-11] The plain char type shall only be used for the storage and use of
/// character values.
class CharStorageVisitor : public TypesVisitorInterface {
  AutocheckDiagnostic &AD;
  clang::ASTContext &AC;

public:
  explicit CharStorageVisitor(AutocheckDiagnostic &AD, clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *ICE) override;

private:
  bool isPlainChar(const clang::QualType &Type) const;
};

/// [M5-0-12] Signed char and unsigned char type shall only be used for the
/// storage and use of numeric values.
class SignCharStorageVisitor : public TypesVisitorInterface {
  AutocheckDiagnostic &AD;
  clang::ASTContext &AC;

public:
  explicit SignCharStorageVisitor(AutocheckDiagnostic &AD,
                                  clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *ICE) override;

private:
  bool isPlainChar(const clang::QualType &Type) const;
  bool isUnsignedOrSignedChar(const clang::QualType &Type) const;
};

/// [A5-0-2] The condition of an if-statement and the condition of an iteration
/// statement shall have type bool.
class ConditionNotBoolVisitor : public TypesVisitorInterface {
  AutocheckDiagnostic &AD;
  clang::BeforeThanCompare<clang::SourceLocation> IsBefore;
  clang::SourceRange ConditionSourceRange;

public:
  explicit ConditionNotBoolVisitor(AutocheckDiagnostic &AD);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitIfStmt(clang::IfStmt *IS) override;
  bool VisitWhileStmt(clang::WhileStmt *WS) override;
  bool VisitForStmt(clang::ForStmt *FS) override;
  bool VisitDoStmt(clang::DoStmt *DS) override;
  bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *ICE) override;

private:
  /// Traverse the condition part and try to find IntegralToBoolean cast.
  void diagnoseCondition(clang::Expr *Condition);
};

/// [A2-13-3] Type wchar_t shall not be used.
class TypeWchartVisitor : public TypesVisitorInterface {
  AutocheckDiagnostic &AD;
  clang::ASTContext &AC;

public:
  explicit TypeWchartVisitor(AutocheckDiagnostic &AD, clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitTypeLoc(const clang::TypeLoc &TL) override;
};

/// [A18-1-1] C-style arrays shall not be used.
class CStyleArrayVisitor : public TypesVisitorInterface {
  AutocheckDiagnostic &AD;
  clang::ASTContext &AC;
  /// TypeLoc of the last constexpr declared variable which should be exempt
  /// from the rule.
  clang::TypeLoc IgnoredTypeLoc;

public:
  explicit CStyleArrayVisitor(AutocheckDiagnostic &AD, clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitVarDecl(const clang::VarDecl *VD) override;
  bool VisitCXXNewExpr(const clang::CXXNewExpr *NE) override;
  bool VisitTypeLoc(const clang::TypeLoc &TL) override;
};

/// [A18-1-2] The std::vector<bool> specialization shall not be used.
class BoolVectorUsedVisitor : public TypesVisitorInterface {
  AutocheckDiagnostic &AD;
  clang::ASTContext &AC;

public:
  explicit BoolVectorUsedVisitor(AutocheckDiagnostic &AD,
                                 clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitTypeLoc(const clang::TypeLoc &TL) override;
};

/// [A0-4-2] Type long double shall not be used.
class TypeLongDoubleVisitor : public TypesVisitorInterface {
  AutocheckDiagnostic &AD;
  clang::ASTContext &AC;

public:
  explicit TypeLongDoubleVisitor(AutocheckDiagnostic &AD,
                                 clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitTypeLoc(const clang::TypeLoc &TL) override;
};

/// [A18-1-3] The std::auto_ptr type shall not be used.
class AutoPtrVisitor : public TypesVisitorInterface {
  AutocheckDiagnostic &AD;
  clang::ASTContext &AC;

public:
  explicit AutoPtrVisitor(AutocheckDiagnostic &AD, clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitTypeLoc(const clang::TypeLoc &TL) override;
};

/// [A5-1-7] A lambda shall not be an operand to decltype or typeid.
class DecltypeTypeidVisitor : public TypesVisitorInterface {
  AutocheckDiagnostic &AD;
  clang::ASTContext &AC;

public:
  explicit DecltypeTypeidVisitor(AutocheckDiagnostic &AD,
                                 clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitTypeLoc(const clang::TypeLoc &TL) override;
  bool VisitCXXTypeidExpr(const clang::CXXTypeidExpr *CTE) override;

private:
  bool IsLambda(const clang::VarDecl *VD) const;
  const clang::DeclRefExpr *GetDRE(const clang::Expr *E) const;
  const clang::VarDecl *GetVarDecl(const clang::DeclRefExpr *DRE) const;
};

/// Main visitor for type related checks. Makes an instance of every class that
/// implement a TypesVisitorInterface if appropriate flag is found. Runs all
/// Type Visitors with one AST traversal.
class TypesVisitor : public clang::RecursiveASTVisitor<TypesVisitor> {
  AutocheckDiagnostic &AD;

  std::forward_list<std::unique_ptr<TypesVisitorInterface>> AllVisitors;

public:
  explicit TypesVisitor(AutocheckDiagnostic &AD, clang::ASTContext &AC);
  void run(clang::TranslationUnitDecl *TUD);

  bool TraverseDecl(clang::Decl *D);

  bool VisitImplicitCastExpr(const clang::ImplicitCastExpr *ICE);
  bool VisitIfStmt(clang::IfStmt *IS);
  bool VisitWhileStmt(clang::WhileStmt *WS);
  bool VisitForStmt(clang::ForStmt *FS);
  bool VisitDoStmt(clang::DoStmt *DS);
  bool VisitTypeLoc(const clang::TypeLoc &TL);
  bool VisitVarDecl(const clang::VarDecl *VD);
  bool VisitCXXNewExpr(const clang::CXXNewExpr *NE);
  bool VisitCXXTypeidExpr(const clang::CXXTypeidExpr *CTE);
};

} // namespace autocheck

#endif
