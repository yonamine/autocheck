//===--- ClassesVisitor.h - Visitor for class related checks --------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file performs checks for rules related to classes by traversing the
// clang AST.
//
// This implements the following checks:
// - [A0-1-3]  Every function defined in an anonymous namespace, or static
//             function with internal linkage, or private member function shall
//             be used.
// - [M8-3-1]  Parameters in an overriding virtual function shall either use the
//             same default arguments as the function they override, or else
//             shall not specify any default arguments.
// - [M9-3-1]  Const member functions shall not return non-const pointers or
//             references to class-data.
// - [A9-5-1]  Unions shall not be used.
// - [M10-1-1] Classes should not be derived from virtual bases.
// - [A10-3-1] Virtual function declaration shall contain exactly one of the
//             three specifiers: (1) virtual, (2) override, (3) final.
// - [A10-3-2] Each overriding virtual function shall be declared with the
//             override or final specifier.
// - [A10-3-3] Virtual functions shall not be introduced in a final class.
// - [M10-3-3] A virtual function shall only be overridden by a pure virtual
//             function if it is itself declared as pure virtual.
// - [A11-0-1] A non-POD type should be defined as class.
// - [M11-0-1] Member data in non-POD class types shall be private.
// - [A12-1-4] All constructors that are callable with a single argument of
//             fundamental type shall be declared explicit.
// - [A12-4-2] If a public destructor of a class is non-virtual, then the class
//             should be declared final.
// - [A12-8-2] User-defined copy and move assignment operators should use
//             user-defined no-throw swap function.
// - [A13-2-3] A relational operator shall return a boolean value.
// - [A13-5-1] If "operator[]" is to be overloaded with a non-const version,
//             const version shall also be implemented.
// - [A13-5-2] All user-defined conversion operators shall be defined explicit.
// - [A13-5-3] User-defined conversion operators should not be used.
//
//===----------------------------------------------------------------------===//

#ifndef AST_CLASSES_VISITOR_H
#define AST_CLASSES_VISITOR_H

#include "AutocheckContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include <forward_list>
#include <set>
#include <stack>

namespace autocheck {

/// Common interface for all AutosarClasses visitors.
class ClassesVisitorInterface {
public:
  virtual ~ClassesVisitorInterface();

  /// Custom hook function called after the traversal is done and visitor was
  /// not interrupted early (e.g. warning limit was reached)
  virtual void PostWork();

  /// Custom traversal function called for each Decl before it and its subtree
  /// are visited.
  virtual bool PreTraverseDecl(clang::Decl *D);
  /// Custom traversal function called for each Decl after it and its entire
  /// subtree have been visited.
  virtual bool PostTraverseDecl(clang::Decl *D);

  virtual bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD);
  virtual bool VisitCXXMethodDecl(const clang::CXXMethodDecl *CMD);
  virtual bool VisitCXXConversionDecl(const clang::CXXConversionDecl *CCD);
  virtual bool VisitCXXMemberCallExpr(const clang::CXXMemberCallExpr *CMCE);
};

/// [M10-1-1] Classes should not be derived from virtual bases.
class DerivedFromVirtualVisitor : public ClassesVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit DerivedFromVirtualVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) override;
};

/// [A10-3-1] Virtual function declaration shall contain exactly one of the
/// three specifiers: (1) virtual, (2) override, (3) final.
class VirtualFuncSpecifierVisitor : public ClassesVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit VirtualFuncSpecifierVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXMethodDecl(const clang::CXXMethodDecl *CMD) override;
};

/// [A10-3-2] Each overriding virtual function shall be declared with the
/// override or final specifier.
class OverrideFuncSpecifierVisitor : public ClassesVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit OverrideFuncSpecifierVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXMethodDecl(const clang::CXXMethodDecl *CMD) override;
};

/// [A10-3-3] Virtual functions shall not be introduced in a final class.
class VirtualMethodFinalClassVisitor : public ClassesVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit VirtualMethodFinalClassVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) override;
};

/// [M10-3-3] A virtual function shall only be overridden by a pure virtual
/// function if it is itself declared as pure virtual.
class PureFuncOverrideVisitor : public ClassesVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit PureFuncOverrideVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXMethodDecl(const clang::CXXMethodDecl *CMD) override;
};

/// [M11-0-1] Member data in non-POD class types shall be private.
class MemberDataPrivateVisitor : public ClassesVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit MemberDataPrivateVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) override;
};

/// [A11-0-1] A non-POD type should be defined as class.
class NonPODAsClassVisitor : public ClassesVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit NonPODAsClassVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) override;
};

/// [A12-1-4] All constructors that are callable with a single argument of
/// fundamental type shall be declared explicit.
class ExplicitCtorsVisitor : public ClassesVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit ExplicitCtorsVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) override;
};

/// [A12-4-2] If a public destructor of a class is non-virtual, then the class
/// should be declared final.
class FinalClassDestructorVisitor : public ClassesVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit FinalClassDestructorVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) override;
};

/// [A13-2-3] A relational operator shall return a boolean value.
class RelOpBoolValueVisitor : public ClassesVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit RelOpBoolValueVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXMethodDecl(const clang::CXXMethodDecl *CMD);
};

/// [A13-5-2] All user-defined conversion operators shall be defined explicit.
class ImplicitConversionOpVisitor : public ClassesVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit ImplicitConversionOpVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXConversionDecl(const clang::CXXConversionDecl *CMD) override;
};

/// [A13-5-3] User-defined conversion operators should not be used.
class ConverionOpUsedVisitor : public ClassesVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit ConverionOpUsedVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXConversionDecl(const clang::CXXConversionDecl *CMD) override;
};

/// [A13-5-1] If "operator[]" is to be overloaded with a non-const version,
/// const version shall also be implemented.
class SubscriptOperatorOverloadVisitor : public ClassesVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit SubscriptOperatorOverloadVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) override;
};

/// [A0-1-3] Every function defined in an anonymous namespace, or static
/// function with internal linkage, or private member function shall be used.
class UnusedPrivateMethodVisitor : public ClassesVisitorInterface {
  clang::DiagnosticsEngine &DE;
  std::set<const clang::CXXMethodDecl *> PrivateMethods;

public:
  explicit UnusedPrivateMethodVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  /// Emits warnings for all unused private methods. Should be called after the
  /// traversal is done.
  void PostWork() override;
  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) override;
  bool VisitCXXMemberCallExpr(const clang::CXXMemberCallExpr *CMCE) override;
};

/// [M9-3-1] Const member functions shall not return non-const pointers or
/// references to class-data.
class NonConstReturnedFromConst : public ClassesVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit NonConstReturnedFromConst(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) override;
};

/// [A12-8-2] User-defined copy and move assignment operators should use
/// user-defined no-throw swap function.
class CopyAndMoveAssignmentNoSwapVisitor : public ClassesVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit CopyAndMoveAssignmentNoSwapVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);
  bool VisitCXXMethodDecl(const clang::CXXMethodDecl *CRD) override;
};

/// [A9-5-1] Unions shall not be used.
class UnionsUsedVisitor : public ClassesVisitorInterface {
private:
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &AC;

public:
  explicit UnionsUsedVisitor(clang::DiagnosticsEngine &DE,
                             clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *D) override;
};

class ChangedDefaultArgumentsVisitor : public ClassesVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &ASTCtx;
  // Set of locations where warning should be shown.
  std::set<clang::SourceLocation> Locations;
  clang::Expr::EvalResult ResultOfOverridenExpr;
  clang::Expr::EvalResult ResultOfOverridingExpr;

  void evaluteResults(const clang::Expr *E1, const clang::Expr *E2,
                      clang::Expr::EvalResult &Result1,
                      clang::Expr::EvalResult &Result2);
  bool compareResults(clang::Expr::EvalResult &Result1,
                      clang::Expr::EvalResult &Result2, clang::QualType Ty);

  // Obtain first clang::CXXConstructExpr which is descendent of 'E' but is not
  // move constructor.
  const clang::CXXConstructExpr *getCXXConstructExpr(const clang::Expr *E);

  // Check if overriden parameter is the same as overriding one.
  // Note that if overriding parameter doesn't have default argument, it is
  // compliant no matter whether overriden method has default argument or not.
  void checkParameters(clang::ParmVarDecl *OverridenParamVarDecl,
                       clang::ParmVarDecl *OverridingParamVarDecl);

  // Handle case where both expressions are unevaluatable.
  void handleUnevaluatableArguments(const clang::Expr *OverridenDefaultArgExpr,
                                    const clang::Expr *OverridingDefaultArgExpr,
                                    const clang::SourceLocation &Loc);

  // Handle case where both expressions are evaluatable.
  void handleEvaluatableArguments(const clang::Expr *OverridenDefaultArgExpr,
                                  const clang::Expr *OverridingDefaultArgExpr,
                                  const clang::SourceLocation &Loc);

public:
  explicit ChangedDefaultArgumentsVisitor(clang::DiagnosticsEngine &DE,
                                          clang::ASTContext &ASTCtx)
      : DE(DE), ASTCtx(ASTCtx) {}
  static bool isFlagPresent(const AutocheckContext &Context);
  bool VisitCXXMethodDecl(const clang::CXXMethodDecl *CRD) override;
};

/// Main visitor for AutosarClasses. Makes an instance of every class that
/// implement a ClassesVisitorInterface if appropriate flag is found. Runs all
/// Class Visitors with one AST traversal.
class ClassesVisitor : public clang::RecursiveASTVisitor<ClassesVisitor> {
  clang::DiagnosticsEngine &DE;

  std::forward_list<std::unique_ptr<ClassesVisitorInterface>> AllVisitors;

public:
  explicit ClassesVisitor(clang::DiagnosticsEngine &DE,
                          clang::ASTContext &ASTCtx);
  void run(clang::TranslationUnitDecl *TUD);

  bool TraverseDecl(clang::Decl *D);
  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD);
  bool VisitCXXMethodDecl(const clang::CXXMethodDecl *CMD);
  bool VisitCXXConversionDecl(const clang::CXXConversionDecl *CCD);
  bool VisitCXXMemberCallExpr(const clang::CXXMemberCallExpr *CMCE);
};
} // namespace autocheck

#endif
