//===--- DeclarationsVisitor.h - Visitor for declaration related checks ---===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file performs checks for rules related to declarations by traversing the
// clang AST.
//
// This implements the following checks:
// - [A0-1-6]  There should be no unused type declarations.
// - [M3-1-2]  Functions shall not be declared at block scope.
// - [A3-1-4]  When an array with external linkage is declared, its size shall
//             be stated explicitly.
// - [A3-3-2]  Static and thread-local objects shall be constant-initialized.
// - [M3-3-2]  If a function has internal linkage then all re-declarations shall
//             include the static storage class specifier.
// - [M3-4-1]  An identifier declared to be an object or type shall be defined
//             in a block that minimizes its visibility.
// - [A5-0-3]  The declaration of objects shall contain no more than two levels
//             of pointer indirection.
// - [A5-7-1]  A lambda shall not be an operand to decltype or typeid.
// - [A7-1-1]  Constexpr or const specifiers shall be used for immutable data
//             declaration.
// - [A7-1-3]  CV-qualifiers shall be placed on the right hand side of the type
//             that is a typedef or a using name..
// - [A7-1-6]  The typedef specifier shall not be used.
// - [A7-2-2]  Enumeration underlying base type shall be explicitly defined.
// - [A7-2-3]  Enumerations shall be declared as scoped enum classes.
// - [A7-2-4]  In an enumeration, either (1) none, (2) the first or (3) all
//             enumerators shall be initialized.
// - [M7-3-1]  The global namespace shall only contain main, namespace
//             declarations and extern "C" declarations.
// - [M7-3-2]  The identifier main shall not be used for a function other than
//             the global function main.
// - [A7-3-3]  There shall be no unnamed namespaces in header files.
// - [M7-3-4]  Using-directives shall not be used.
// - [A7-4-1]  The asm declaration shall not be used.
// - [A8-4-1]  Functions shall not be defined using the ellipsis notation.
// - [M8-4-2]  The identifiers used for the parameters in a re-declaration of a
//             function shall be identical to those in the declaration.
// - [A8-4-7]  "in" parameters for "cheap to copy" types shall be passed by
//             value.
// - [A8-5-2]  Braced-initialization {}, without equals sign, shall be used for
//             variable initialization.
// - [M8-5-2]  Braces shall be used to indicate and match the structure in the
//             non-zero initialization of arrays and structures.
// - [A8-5-3]  A variable of type auto shall not be initialized using {} or ={}
//             braced-initialization.
// - [A9-6-1]  Bit-fields shall be either unsigned integral, or enumeration
//             (with underlying type of unsigned integral type).
// - [A11-3-1] Friend declarations shall not be used.
// - [A12-1-2] Both NSDMI and a non-static member initializer in a constructor
//             shall not be used in the same type.
// - [A13-2-1] An assignment operator shall return a reference to "this".
// - [A15-4-2] If a function is declared to be noexcept, noexcept(true) or
//             noexcept(<true condition>), then it shall not exit with an
//             exception.
// - [A15-5-1] All user-provided class destructors, deallocation functions, move
//             constructors, move assignment operators and swap functions shall
//             not exit with an exception. A noexcept exception specification
//             shall be added to these functions as appropriate.
// - [A17-0-4] C-style strings shall not be used.
// - [A18-5-3] The form of delete operator shall match the form of new operator
//             used to allocate the memory.
// - [A26-5-2] Random number engines shall not be default-initialized.
//
//===----------------------------------------------------------------------===//

#ifndef AST_DECLARATIONS_VISITOR_H
#define AST_DECLARATIONS_VISITOR_H

#include "AutocheckContext.h"
#include "Diagnostics/AutocheckDiagnostic.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "llvm/ADT/SmallSet.h"
#include <forward_list>
#include <set>
#include <stack>

namespace autocheck {

/// Common interface for all declaration related visitors.
class DeclarationsVisitorInterface {
public:
  virtual ~DeclarationsVisitorInterface();

  /// Custom hook function called after the traversal is done and visitor was
  /// not interrupted early (e.g. warning limit was reached).
  virtual void PostWork();

  /// Custom traversal function called for each Decl before it and its subtree
  /// are visited.
  virtual bool PreTraverseDecl(clang::Decl *D);
  /// Custom traversal function called for each Decl after it and its entire
  /// subtree have been visited.
  virtual bool PostTraverseDecl(clang::Decl *D);
  /// Custom traversal function called for each Stmt before it and its subtree
  /// are visited.
  virtual bool PreTraverseStmt(clang::Stmt *S);
  /// Custom traversal function called for each Stmt after it and its entire
  /// subtree have been visited.
  virtual bool PostTraverseStmt(clang::Stmt *S);

  virtual bool VisitGCCAsmStmt(const clang::GCCAsmStmt *GAS);
  virtual bool VisitVarDecl(const clang::VarDecl *VD);
  virtual bool VisitFunctionDecl(const clang::FunctionDecl *FD);
  virtual bool VisitTypedefNameDecl(const clang::TypedefNameDecl *TND);
  virtual bool VisitCXXMethodDecl(const clang::CXXMethodDecl *CMD);
  virtual bool VisitUnaryOperator(const clang::UnaryOperator *UO);
  virtual bool VisitBinaryOperator(const clang::BinaryOperator *BO);
  virtual bool VisitCallExpr(const clang::CallExpr *CE);
  virtual bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD);
  virtual bool VisitFieldDecl(const clang::FieldDecl *FD);
  virtual bool VisitDeclRefExpr(const clang::DeclRefExpr *DRE);
  virtual bool VisitCXXConstructExpr(const clang::CXXConstructExpr *CCE);
  virtual bool VisitCXXThrowExpr(const clang::CXXThrowExpr *CTE);
  virtual bool VisitCXXDeleteExpr(const clang::CXXDeleteExpr *CDE);
  virtual bool VisitTypeAliasDecl(const clang::TypeAliasDecl *TAD);
  virtual bool VisitTypedefDecl(const clang::TypedefDecl *TD);
  virtual bool VisitFriendDecl(const clang::FriendDecl *TD);
  virtual bool VisitEnumDecl(const clang::EnumDecl *ED);
  virtual bool VisitNamespaceDecl(const clang::NamespaceDecl *ND);
  virtual bool VisitUsingDirectiveDecl(const clang::UsingDirectiveDecl *UDD);
  virtual bool VisitTranslationUnitDecl(const clang::TranslationUnitDecl *TUD);
};

/// [A7-4-1] The asm declaration shall not be used.
class ASMDeclarationUsedVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit ASMDeclarationUsedVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitGCCAsmStmt(const clang::GCCAsmStmt *GAS) override;
};

/// [A8-4-1] Functions shall not be defined using the ellipsis notation.
class VariadicFunctionUsedVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit VariadicFunctionUsedVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitFunctionDecl(const clang::FunctionDecl *FD) override;
};

/// [M8-4-2] The identifiers used for the parameters in a re-declaration of a
/// function shall be identical to those in the declaration.
class FunctionRedeclParamsVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit FunctionRedeclParamsVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitFunctionDecl(const clang::FunctionDecl *FD) override;
};

/// [A8-5-3] A variable of type auto shall not be initialized using {} or ={}
/// braced-initialization.
class AutoVarBracedInitVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::Sema &SemaRef;

public:
  explicit AutoVarBracedInitVisitor(clang::DiagnosticsEngine &DE,
                                    clang::Sema &SemaRef);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitVarDecl(const clang::VarDecl *VD) override;
};

/// [A8-5-2] Braced-initialization {}, without equals sign, shall be used for
/// variable initialization.
class VarBracedInitVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &AC;
  clang::Sema &SemaRef;

public:
  explicit VarBracedInitVisitor(clang::DiagnosticsEngine &DE,
                                clang::ASTContext &AC, clang::Sema &SemaRef);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitVarDecl(const clang::VarDecl *VD) override;
};

/// [A0-1-6] There should be no unused type declarations.
class UnusedGlobalTypedef : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  llvm::SmallVector<const clang::TypedefNameDecl *, 0> GlobalTypedefs;

public:
  explicit UnusedGlobalTypedef(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  /// Emits warnings for all unused global typedefs. Should be called after the
  /// traversal is done.
  void PostWork() override;
  bool VisitTypedefNameDecl(const clang::TypedefNameDecl *TND) override;
};

/// [A7-1-3] CV-qualifiers shall be placed on the right hand side of the type
/// that is a typedef or a using name.
class CVQualifiersPlacedLeftVisitor : public DeclarationsVisitorInterface {
  clang::SourceManager &SM;
  clang::DiagnosticsEngine &DE;
  clang::Sema &SemaRef;

public:
  explicit CVQualifiersPlacedLeftVisitor(clang::SourceManager &SM,
                                         clang::DiagnosticsEngine &DE,
                                         clang::Sema &SemaRef);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitVarDecl(const clang::VarDecl *VD) override;
};

/// [A13-2-1] An assignment operator shall return a reference to "this".
class AssignmentOperatorReturn : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit AssignmentOperatorReturn(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXMethodDecl(const clang::CXXMethodDecl *CMD) override;

private:
  bool isWrongReturnStmt(const clang::CXXMethodDecl *CMD) const;
  bool isWrongDeclarationReturnType(const clang::CXXMethodDecl *CMD) const;
};

/// [A7-1-1] Constexpr or const specifiers shall be used for immutable data
/// declaration.
class ConstUnusedForImmutableData : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &AC;
  /// Set of all declared non const or non constexpr variables that are
  /// candidates to be immutable.
  std::set<clang::SourceLocation> ImmutableVarCandidates;

public:
  explicit ConstUnusedForImmutableData(clang::DiagnosticsEngine &DE,
                                       clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  /// Emits warnings for all variables that are considered immutable. Should be
  /// called after the traversal is done.
  void PostWork() override;
  bool VisitVarDecl(const clang::VarDecl *VD) override;
  bool VisitUnaryOperator(const clang::UnaryOperator *UO) override;
  bool VisitBinaryOperator(const clang::BinaryOperator *BO) override;
  bool VisitCallExpr(const clang::CallExpr *CE) override;

private:
  bool CheckFunctionArguments(const clang::CallExpr *CE);
  bool CheckOperatorCallExpr(const clang::CallExpr *CE);
  bool CheckMemberCallExpr(const clang::CallExpr *CE);
  bool CheckIfOperandIsUserDefinedType(const clang::Expr *E);
  bool CheckIfOperandIsArraySubscript(const clang::Expr *E);
  bool CheckIfOperandIsOperatorSubscript(const clang::Expr *E);
  bool CheckIfOperandIsPrimitiveOrPtrOrRefType(const clang::Expr *LHS);
  const clang::DeclRefExpr *FindDREInArraySubscript(const clang::Expr *LHS);
  bool IfLValueEraseFromImmCandidates(const clang::DeclRefExpr *DRE);
};

/// [A12-1-2] Both NSDMI and a non-static member initializer in a constructor
/// shall not be used in the same type.
class NSDMIAndConsutructorInitUsed : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit NSDMIAndConsutructorInitUsed(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) override;
};

/// [M3-4-1] An identifier declared to be an object or type shall be defined in
/// a block that minimizes its visibility.
class BroadScopeIdentifierVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &ASTCtx;

  bool isNewScope(const clang::Stmt *S);

  enum class VarState {
    NotYetReferenced /*Found VarDecl but not a DeclRefExpr yet*/,
    ReferencedDeeper /*Found DeclRefExpr on a lower scope*/,
    ReferencedDeeperAndResurfaced /*Found DeclRefExpr on a lower scope
                                    and returned to scope of VarDecl*/
  };

  struct VarInfo {
    VarInfo(unsigned DeclLevel)
        : DeclLevel(DeclLevel), State(VarState::NotYetReferenced) {}
    const unsigned DeclLevel;
    VarState State;
  };

  bool isForInit = false;
  bool isInFor = false;
  bool isIfCond = false;
  clang::QualType forInitType;

  typedef std::map<const clang::ValueDecl *, VarInfo> VarInfoMap;
  VarInfoMap PotentiallyBadVars;
  llvm::SmallVector<const clang::Stmt *, 16> StmtsForTraversal;

  llvm::SmallVector<const clang::Stmt *, 16> StmtScopes;

public:
  explicit BroadScopeIdentifierVisitor(clang::DiagnosticsEngine &DE,
                                       clang::ASTContext &ASTCtx);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitVarDecl(const clang::VarDecl *VD) override;
  bool VisitDeclRefExpr(const clang::DeclRefExpr *DRE) override;
  bool PreTraverseStmt(clang::Stmt *S) override;
  bool PostTraverseStmt(clang::Stmt *S) override;
  void PostWork() override;

  bool UpdateStatesOnEndOfScope();
  VarInfoMap &getPotentiallyBadVars() { return PotentiallyBadVars; }
  const VarState &getVarState(const VarInfoMap::iterator &It) const {
    return It->second.State;
  }
  void setVarState(const VarInfoMap::iterator &It, VarState State) {
    It->second.State = State;
  }
  unsigned short getDeclLevel(const VarInfoMap::iterator &It) const {
    return It->second.DeclLevel;
  }
};

/// [M8-5-2] Braces shall be used to indicate and match the structure in the
/// non-zero initialization of arrays and structures.
class MissingBracesOrEltsVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::SourceManager &SM;
  clang::Sema &SemaRef;

  struct InitListInfo {
    // This corresponds to first field (in e.g. struct) that was not explicitly
    // initialized using initialization list.
    const clang::ImplicitValueInitExpr *FirstImplValInit;
    // InitList that is missing element or braces. Missing braces warning is
    // reported once all elements are added.
    const clang::InitListExpr *BadILE;
    // Variables used to help us write helpful FixItHint message.
    // Used to determine whether element or braces are missing.
    bool MissingElt;
    // Number of array element present in initialization list.
    unsigned NumOfElts;
  };

  bool hasBraces(const clang::InitListExpr *ILE);
  // Intended use is for arrays only. InitListExpr needs to have exactly one
  // array_filler and exactly one either InitListExpr (recourse) or Literal that
  // holds zero value for array type. Braces are not allowed for
  // sub'InitListExpr'essions.
  bool hasExactlyOneElt(const clang::InitListExpr *ILE);
  // Walk through all children and array_fillers, recourse for InitListExpr.
  // Only ImplicitValueInitExpr and InitListExpr (without braces) are allowed.
  // Caller checks for braces in InitListExpr meaning missing elements are
  // reported first. Top call from visitor doesn't because that one has braces
  // (it is an initialization list).
  bool isEmpty(const clang::InitListExpr *ILE);
  bool isSubExprEmpty(const clang::Stmt *SubStatement);

  bool hasMissingEltOrBraces(const clang::InitListExpr *ILE, InitListInfo &ILI);
  bool reportMissingEltOrBraces(InitListInfo &ILI, std::stringstream &EltHint);

public:
  explicit MissingBracesOrEltsVisitor(clang::DiagnosticsEngine &DE,
                                      clang::Sema &SemaRef);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitVarDecl(const clang::VarDecl *VD) override;
};

/// [A3-3-2] Static and thread-local objects shall be constant-initialized.
class ConstantInitializer : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &AC;
  clang::Sema &SemaRef;

public:
  explicit ConstantInitializer(clang::DiagnosticsEngine &DE,
                               clang::ASTContext &AC, clang::Sema &SemaRef);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitVarDecl(const clang::VarDecl *VD) override;

private:
  bool hasRedeclaration(const clang::VarDecl *VD) const;
};

/// [A26-5-2] Random number engines shall not be default-initialized.
class RandomEngineDefaultInitialized : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

  llvm::StringRef randomDeviceName = "random_device";

  // Random number engines and random number engine adaptors defined inside of
  // <random> header; all predefined generators rely on one of the generators in
  // this list. User should only ever use one of these generators.
  std::vector<std::string> engines = {
      "linear_congruential_engine", "mersenne_twister_engine",
      "subtract_with_carry_engine", "discard_block_engine",
      "independent_bits_engine",    "shuffle_order_engine"};

public:
  explicit RandomEngineDefaultInitialized(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitCXXConstructExpr(const clang::CXXConstructExpr *CCE) override;

private:
  bool isAcceptableEngine(const std::string &Name) const;
};

/// [A5-0-3] The declaration of objects shall contain no more than two levels of
/// pointer indirection.
class MoreThanTwoLevelsOfPointerIndirection
    : public DeclarationsVisitorInterface {

  clang::DiagnosticsEngine &DE;
  clang::ASTContext &AC;

  /// This method helps us get to the pointer part of a type.
  /// For example, int ***[2][3] is seen as an array type, so we have to peel
  /// this type until we reach the part with the pointer indirection. Same goes
  /// for 'reference to pointer' types. For example int ****&.
  clang::QualType getToPointerIndirectionType(clang::QualType QT);

  /// This is the core method of this visitor. For every declaration, after
  /// obtaining the type, this method checks whether it has more than two
  /// pointer indirections.
  bool hasMoreThanTwoPointerIndirections(const clang::QualType QT);

  /// This method is used while visiting pointers to functions.
  /// For example, for int*** (**func)() we have to peel two pointer levels
  /// (**func) to get to FunctionProtoType type int*** (func)() so we can check
  /// the return type.
  const clang::FunctionProtoType *getFunctionProtoType(clang::QualType QT);

  /// This method recursively checks types for pointer indirection
  /// with more than two levels. Recurision is needed to cover cases of nested
  /// types. For example: std::vector<std::vector<int ***> >.
  /// This method also calls reportWarning function if type violates this rule,
  /// and its return value is used to determine whether visitor should be
  /// stopped.
  bool checkType(clang::QualType QT, const clang::SourceLocation &SL);

public:
  explicit MoreThanTwoLevelsOfPointerIndirection(clang::DiagnosticsEngine &DE,
                                                 clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitVarDecl(const clang::VarDecl *VD) override;
  bool VisitFunctionDecl(const clang::FunctionDecl *FD) override;
  bool VisitFieldDecl(const clang::FieldDecl *FD) override;
};

/// [A15-4-2] If a function is declared to be noexcept, noexcept(true) or
/// noexcept(<true condition>), then it shall not exit with an exception.
class NoExceptionVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  llvm::SmallVector<const clang::FunctionDecl *, 4> FunctionDeclStack;
  llvm::SmallVector<const clang::CXXTryStmt *, 4> CXXTryStmtStack;

public:
  explicit NoExceptionVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool PreTraverseDecl(clang::Decl *D) override;
  bool PostTraverseDecl(clang::Decl *D) override;
  bool PreTraverseStmt(clang::Stmt *S) override;
  bool PostTraverseStmt(clang::Stmt *S) override;

  bool VisitCallExpr(const clang::CallExpr *CE) override;
  bool VisitCXXThrowExpr(const clang::CXXThrowExpr *CTE) override;
};

/// [A15-5-1] All user-provided class destructors, deallocation functions, move
/// constructors, move assignment operators and swap functions shall not exit
/// with an exception. A noexcept exception specification shall be added to
/// these functions as appropriate.
class ExitedWithExceptionVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  const clang::FunctionDecl *InFunction = nullptr;
  llvm::SmallVector<const clang::CXXTryStmt *, 4> CXXTryStmtStack;

  bool isTemplatedWithDependentNoexcept(const clang::CXXMethodDecl *MD);

public:
  explicit ExitedWithExceptionVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool PreTraverseDecl(clang::Decl *D) override;
  bool PostTraverseDecl(clang::Decl *D) override;
  bool PreTraverseStmt(clang::Stmt *S) override;
  bool PostTraverseStmt(clang::Stmt *S) override;
  bool VisitCXXThrowExpr(const clang::CXXThrowExpr *CTE) override;
};

/// [A18-5-3], the form of delete operator shall match the form of new operator
/// used to allocate the memory.
class MismatchedNewDeleteVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

  /// Set of pointer variables dynamically allocated with the 'new' operator.
  std::map<const clang::VarDecl *, const clang::CXXNewExpr *> VarDeclsWithNew;

  /// Set of pointer fields dynamically allocated with the 'new' operator.
  std::map<const clang::FieldDecl *, const clang::CXXNewExpr *> FieldsWithNew;

public:
  explicit MismatchedNewDeleteVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  /// Collects variables initialized with CXXNewExpr. Will report mismatches of
  /// 'new' and 'new[]' inside same InitListExpr.
  bool VisitVarDecl(const clang::VarDecl *VD) override;

  /// Collects variables and fields (re)initialized with CXXNewExpr. Will report
  /// mismatches of 'new' and 'new[]' inside same InitListExpr.
  bool VisitBinaryOperator(const clang::BinaryOperator *BO) override;

  /// Checks for mismatches of different forms of CXXNewExpr and CXXDeleteExpr.
  bool VisitCXXDeleteExpr(const clang::CXXDeleteExpr *CDE) override;

  /// Collects fields initialized with CXXNewExpr both with in-class
  /// initializers and constructors. Will report mismatches of 'new' and 'new[]'
  /// inside same InitListExpr.
  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) override;

private:
  /// Check if Expr E is (re)initalized with matching 'new' operator or
  /// destroyed with matching 'delete' operator by Expr NewOrDelete. Returns a
  /// CXXNewExpr that mismatches if it exists.
  const clang::CXXNewExpr *checkMismatch(const clang::Expr *E,
                                         const clang::Expr *NewOrDelete,
                                         bool isArray);

  /// Check if Decl D (which can be either VarDecl or FieldDecl) is
  /// (re)initalized with matching 'new' operator or destroyed with matching
  /// 'delete' operator by Expr NewOrDelete. Returns a CXXNewExpr that
  /// mismatches if it exists.
  template <class T>
  const clang::CXXNewExpr *checkDeclMismatch(
      std::map<const T *, const clang::CXXNewExpr *> &DeclsWithNew,
      const T *Decl, const clang::Expr *NewOrDelete, bool isArray);

  /// If this expression is ArraySubscriptExpr then get it's underneath Expr.
  const clang::Expr *ignoreArraySubscript(const clang::Expr *E);

  /// Returns a pair of CXXNewExprs. First one will be the first found
  /// CXXNewExpr if one exists, otherwise it will be a nullptr. Second one will
  /// be the a CXXNewExpr that uses different form of 'new' from the first one
  /// if it exists, otherwise it will be a nullptr. Argument FirstNew is used
  /// for tracking internal state between recursion calls and should be ignored.
  std::pair<const clang::CXXNewExpr *, const clang::CXXNewExpr *>
  getNewExpr(const clang::Stmt *Init,
             const clang::CXXNewExpr *FirstNew = nullptr);

  /// Reports a warning with given nodes. Retuns false if limit was reached.
  bool reportWarning(const clang::CXXNewExpr *Prev, const clang::Expr *Curr);
};

/// [A27-0-4] C-style strings shall not be used.
class CStyleStringUsed : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &AC;

  llvm::SmallSet<const clang::ValueDecl *, 16> PotentiallyBadVars;

public:
  explicit CStyleStringUsed(clang::DiagnosticsEngine &DE,
                            clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitVarDecl(const clang::VarDecl *VD) override;
  bool VisitCallExpr(const clang::CallExpr *CE) override;
  bool VisitDeclRefExpr(const clang::DeclRefExpr *DRE) override;

private:
  bool isCharPtrOrArray(const clang::QualType &T) const;
};

/// [A5-1-7] A lambda shall not be an operandto decltype or typeid.
class DecltypeTypeidVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &AC;

public:
  explicit DecltypeTypeidVisitor(clang::DiagnosticsEngine &DE,
                                 clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitTypeAliasDecl(const clang::TypeAliasDecl *TAD) override;
  bool VisitVarDecl(const clang::VarDecl *VD) override;

private:
  bool CheckType(const clang::QualType &QT) const;
  bool IsLambda(const clang::VarDecl *VD) const;
  const clang::DeclRefExpr *GetDRE(const clang::Expr *E) const;
  const clang::VarDecl *GetVarD(const clang::DeclRefExpr *DRE) const;
};

/// [A3-1-4] When an array with external linkage is declared, its size shall be
/// stated explicitly.
class ExternArrayImplicitSizeVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &AC;

public:
  explicit ExternArrayImplicitSizeVisitor(clang::DiagnosticsEngine &DE,
                                          clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitVarDecl(const clang::VarDecl *VD) override;
};

/// [A7-1-6] The typedef specifier shall not be used.
class TypedefUsedVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit TypedefUsedVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitTypedefDecl(const clang::TypedefDecl *TD) override;
};

/// [A11-3-1] Friend declarations shall not be used.
class FriendDeclUsedVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit FriendDeclUsedVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitFriendDecl(const clang::FriendDecl *FD) override;
};

/// [A7-2-2] Enumeration underlying base type shall be explicitly defined.
class EnumTypeNotDefinedVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit EnumTypeNotDefinedVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitEnumDecl(const clang::EnumDecl *ED) override;
};

/// [M3-1-2] Functions shall not be declared at block scope.
class BlockScopeFunctionVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit BlockScopeFunctionVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitFunctionDecl(const clang::FunctionDecl *FD) override;
};

/// [M3-3-2] If a function has internal linkage then all re-declarations shall
/// include the static storage class specifier.
class StaticFunctionRedeclarationVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit StaticFunctionRedeclarationVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitFunctionDecl(const clang::FunctionDecl *FD) override;
};

/// [A7-2-3] Enumerations shall be declared as scoped enum classes.
class EnumDeclaredScopedVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit EnumDeclaredScopedVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitEnumDecl(const clang::EnumDecl *ED) override;
};

/// [A7-2-4] In an enumeration, either (1) none, (2) the first or (3) all
/// enumerators shall be initialized.
class EnumConstantInitVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit EnumConstantInitVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitEnumDecl(const clang::EnumDecl *ED) override;
};

/// [M7-3-2] The identifier main shall not be used for a function other than the
/// global function main.
class MainReusedVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit MainReusedVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitFunctionDecl(const clang::FunctionDecl *FD) override;
};

/// [A7-3-3] There shall be no unnamed namespaces in header files.
class UnnamedNamespaceInHeaderVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit UnnamedNamespaceInHeaderVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitNamespaceDecl(const clang::NamespaceDecl *ND) override;
};

/// [M7-3-4] Using-directives shall not be used.
class UsingDirectiveVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit UsingDirectiveVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitUsingDirectiveDecl(const clang::UsingDirectiveDecl *UDD) override;
};

/// [M7-3-1] The global namespace shall only contain main, namespace
/// declarations and extern "C" declarations.
class GlobalNamespaceVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit GlobalNamespaceVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitTranslationUnitDecl(const clang::TranslationUnitDecl *TUD) override;

private:
  static const std::map<std::string, std::string> AllowedTypedefs;
};

/// [A9-6-1] Bit-fields shall be either unsigned integral, or enumeration (with
/// underlying type of unsigned integral type).
class BitFieldTypeVisitor : public DeclarationsVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit BitFieldTypeVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitFieldDecl(const clang::FieldDecl *FD) override;
};

/// Main visitor for declaration related checks. Makes an instance of every
/// class that implement a DeclarationsVisitorInterface if appropriate flag is
/// found. Runs all Declaration Visitors with one AST traversal.
class DeclarationsVisitor
    : public clang::RecursiveASTVisitor<DeclarationsVisitor> {
  clang::DiagnosticsEngine &DE;

  std::forward_list<std::unique_ptr<DeclarationsVisitorInterface>> AllVisitors;

public:
  explicit DeclarationsVisitor(clang::DiagnosticsEngine &DE,
                               clang::ASTContext &AC, clang::Sema &SemaRef);
  void run(clang::TranslationUnitDecl *TUD);

  bool TraverseDecl(clang::Decl *D);
  bool TraverseStmt(clang::Stmt *S);

  bool VisitGCCAsmStmt(const clang::GCCAsmStmt *GAS);
  bool VisitVarDecl(const clang::VarDecl *VD);
  bool VisitFunctionDecl(const clang::FunctionDecl *FD);
  bool VisitTypedefNameDecl(const clang::TypedefNameDecl *TND);
  bool VisitCXXMethodDecl(const clang::CXXMethodDecl *CMD);
  bool VisitUnaryOperator(const clang::UnaryOperator *UO);
  bool VisitBinaryOperator(const clang::BinaryOperator *BO);
  bool VisitCallExpr(const clang::CallExpr *CE);
  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CMD);
  bool VisitFieldDecl(const clang::FieldDecl *FD);
  bool VisitDeclRefExpr(const clang::DeclRefExpr *DRE);
  bool VisitCXXConstructExpr(const clang::CXXConstructExpr *CCE);
  bool VisitCXXThrowExpr(const clang::CXXThrowExpr *CTE);
  bool VisitCXXDeleteExpr(const clang::CXXDeleteExpr *CDE);
  bool VisitTypeAliasDecl(const clang::TypeAliasDecl *TAD);
  bool VisitTypedefDecl(const clang::TypedefDecl *TD);
  bool VisitFriendDecl(const clang::FriendDecl *FD);
  bool VisitEnumDecl(const clang::EnumDecl *ED);
  bool VisitNamespaceDecl(const clang::NamespaceDecl *ND);
  bool VisitUsingDirectiveDecl(const clang::UsingDirectiveDecl *UDD);
  bool VisitTranslationUnitDecl(const clang::TranslationUnitDecl *TUD);
};

} // namespace autocheck

#endif
