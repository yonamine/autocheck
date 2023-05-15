//===--- LexicalRulesVisitor.h - Visitor for lexer related checks ---------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file performs checks for rules related to the lexer by traversing the
// clang AST.
//
// This implements the following checks:
// - [M2-10-1] Different identifiers shall be typographically unambiguous.
// - [A2-10-6] A class or enumeration name shall not be hidden by a variable,
//             function or enumerator declaration in the same scope.
// - [M2-13-3] A "U" suffix shall be applied to all octal or hexadecimal integer
//             literals of unsigned type.
// - [A3-9-1]  Fixed width integer types from <cstdint>, indicating the size and
//             signedness, shall be used in place of the basic numerical types.
// - [A5-1-3]  Parameter list (possibly empty) shall be included in every lambda
//             expression.
// - [M6-2-3]  Before preprocessing, a null statement shall only occur on a line
//             by itself; it may be followed by a comment, provided that the
//             first character following the null statement is a white-space
//             character.
// - [M6-4-1]  An if ( condition ) construct shall be followed by a compound
//             statement. The else keyword shall be followed by either a
//             compound statement, or another if statement.
// - [M7-3-6]  Using-directives and using-declarations (excluding class scope or
//             function scope using-declarations) shall not be used in header
//             files.
// - [A7-1-7]  Each expression statement and identifier declaration shall be
//             placed on a separate line.
//
//===----------------------------------------------------------------------===//

#ifndef AST_LEXICAL_RULES_VISITOR_H
#define AST_LEXICAL_RULES_VISITOR_H

#include "AutocheckContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/ADT/SmallSet.h"
#include <forward_list>

namespace autocheck {

/// Common interface for all AutosarLexicalRules visitors.
class LexicalRulesVisitorInterface {
public:
  virtual ~LexicalRulesVisitorInterface();

  /// Called for each Decl before it and it's subtree is visited.
  virtual bool PreTraverseDecl(clang::Decl *D);
  /// Called for each Decl after it and it's entire subtree has been visited.
  virtual bool PostTraverseDecl(clang::Decl *D);

  virtual bool VisitIntegerLiteral(const clang::IntegerLiteral *IL);
  virtual bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD);
  virtual bool VisitTypedefDecl(const clang::TypedefDecl *TD);
  virtual bool VisitNamespaceDecl(const clang::NamespaceDecl *ND);
  virtual bool VisitVarDecl(const clang::VarDecl *VD);
  virtual bool VisitFunctionDecl(const clang::FunctionDecl *FD);
  virtual bool VisitFieldDecl(const clang::FieldDecl *FD);
  virtual bool VisitCXXMethodDecl(const clang::CXXMethodDecl *CMD);
  virtual bool VisitEnumDecl(const clang::EnumDecl *ED);
  virtual bool VisitEnumConstantDecl(const clang::EnumConstantDecl *ECD);
  virtual bool VisitCompoundStmt(const clang::CompoundStmt *CS);
  virtual bool VisitForStmt(const clang::ForStmt *FS);
  virtual bool VisitWhileStmt(const clang::WhileStmt *WS);
  virtual bool VisitDoStmt(const clang::DoStmt *DS);
  virtual bool VisitIfStmt(const clang::IfStmt *IS);
  virtual bool VisitLambdaExpr(const clang::LambdaExpr *LE);
  virtual bool VisitNullStmt(const clang::NullStmt *NS);
  virtual bool VisitTypeLoc(const clang::TypeLoc &TL);
  virtual bool VisitTranslationUnitDecl(const clang::TranslationUnitDecl *TUD);
  virtual bool VisitUsingDecl(const clang::UsingDecl *UD);
  virtual bool VisitUsingDirectiveDecl(const clang::UsingDirectiveDecl *UDD);
  virtual bool VisitDeclStmt(const clang::DeclStmt *DS);
  virtual bool VisitCXXForRangeStmt(const clang::CXXForRangeStmt *CFRS);
  virtual bool VisitDecl(const clang::Decl *D);
  virtual bool VisitStmt(const clang::Stmt *S);
};

/// [M2-13-3] A "U" suffix shall be applied to all octal or hexadecimal integer
/// literals of unsigned type.
class UnsignedLiterals : public LexicalRulesVisitorInterface {
  clang::DiagnosticsEngine &DE;
  const AutocheckContext &Context;
  clang::ASTContext &AC;

public:
  explicit UnsignedLiterals(clang::DiagnosticsEngine &DE,
                            const AutocheckContext &Context,
                            clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitIntegerLiteral(const clang::IntegerLiteral *IL) override;

private:
  /// Check if integer literal location points to octal or hexadecimal literal.
  bool
  isIntegerLiteralOctalorHexadecimal(const clang::IntegerLiteral *IL) const;
};

typedef llvm::SmallVector<std::pair<std::string, clang::SourceLocation>, 0>
    IdentifierVector;
typedef llvm::SmallSet<std::string, 0> IdentifierSet;

/// [M2-10-1] Different identifiers shall be typographically unambiguous.
class SimilarIdentifiersVisitor : public LexicalRulesVisitorInterface {
  clang::DiagnosticsEngine &DE;
  const AutocheckContext &Context;
  clang::SourceManager &SM;

private:
  /// Used to keep track of different levels of scopes. Each new class,
  /// function, method or block of code is a different scope level.
  class IdScopes {
    /// All identifiers data found on one level of scope.
    struct ScopeData {
      clang::SourceRange RangeOfScope; /// Scope boundaries.
      IdentifierVector
          LocalVarIdentifiers; /// All var identifiers on current scope
      IdentifierVector
          LocalTypeIdentifiers; /// All type identifiers on current scope
      IdentifierVector LocalNamespaceIdentifiers; /// All namespace identifiers
                                                  /// on current scope
      IdentifierSet ForwardDeclarations;    /// Class or function forward decls.
      IdentifierSet OverloadingIdentifiers; /// Method or function IDs.

      explicit ScopeData(clang::SourceRange Range) : RangeOfScope(Range) {}

      const clang::SourceRange &getRange() const { return RangeOfScope; }
      IdentifierVector &getLocalVarIdentifiers() { return LocalVarIdentifiers; }
      IdentifierVector &getLocalTypeIdentifiers() {
        return LocalTypeIdentifiers;
      }
      IdentifierVector &getLocalNamespaceIdentifiers() {
        return LocalNamespaceIdentifiers;
      }
      IdentifierSet &getForwardDeclarations() { return ForwardDeclarations; }
      IdentifierSet &getOverloadingIdentifiers() {
        return OverloadingIdentifiers;
      }
    };

    llvm::SmallVector<ScopeData, 0> ScopeVector;
    clang::BeforeThanCompare<clang::SourceLocation>
        isBefore; /// Compares source locations.

  public:
    explicit IdScopes(clang::SourceManager &SrcMgr);

    using iterator = llvm::SmallVector<ScopeData, 0>::iterator;
    iterator begin() { return ScopeVector.begin(); }
    iterator end() { return ScopeVector.end(); }

    ScopeData &getGlobalScope() { return ScopeVector.front(); }
    ScopeData &getLastScope() { return ScopeVector.back(); }
    bool empty() { return ScopeVector.empty(); }

    /// Makes a new scope level with given range.
    void addNewScopeLevel(clang::SourceRange Range);

    /// Update stack of scopes by checking current SourceLocation against
    /// previous SourceRanges on ScopeVector.
    void update(const clang::SourceLocation &CurrentLocation);

    /// Check if a SourceLocation is in given range.
    bool isInSourceRange(const clang::SourceLocation &BeginLoc,
                         const clang::SourceLocation &EndLoc,
                         const clang::SourceLocation &Location) const;

    /// Check if a SourceLocation is inside given SourceRange.
    inline bool isInSourceRange(const clang::SourceRange &Range,
                                const clang::SourceLocation &Location) const;
  };

  IdScopes Scopes; /// Stack of scopes.

public:
  explicit SimilarIdentifiersVisitor(clang::DiagnosticsEngine &DE,
                                     const AutocheckContext &Context);

  static bool isFlagPresent(const AutocheckContext &Context);

  /// Each function definition is a seperate scope for itself. Checks the name
  /// of the function against global identifiers unless it was forward declared
  /// or overloaded. CXXMethods are checked separately.
  bool VisitFunctionDecl(const clang::FunctionDecl *FD) override;

  /// Each CXXRecord (class, struct, union) represents a seperate scope.
  /// Because of possibility of nested CXXRecords we need to handle their
  /// scopes properly. Scope changes are handled upon entering new record by
  /// checking current SourceLocation against previous CXXRecord's SourceRange.
  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) override;

  /// Upon finding new FieldDecl we first need to update IdScopes stack
  /// in case we exited a nested CXXRecord. Afterward we check FieldDecl's
  /// name against list of current and above scopes before adding that name
  /// to identifiers defined in current scope. Also we need to check if any
  /// subclass has similar filed or method.
  bool VisitFieldDecl(const clang::FieldDecl *FD) override;

  /// Upon finding new MethodDecl we first need to update IdScopes stack
  /// in case we exited a nested CXXRecord. We need to check if this method
  /// overloads any previous method or c++ operator. In that case checking for
  /// similar identifiers is skiped. Afterward we check MethodDecl's name
  /// against list of current and above scopes before adding that name to
  /// identifiers defined in current scope. Finaly we check if similar name was
  /// used in any subclasses if they exist unless it is an overriding method.
  bool VisitCXXMethodDecl(const clang::CXXMethodDecl *CMD) override;

  /// Upon finding new MethodDecl or FieldDecl we first need to update
  /// IdScopes stack in case we exited a nested CXXRecord. Afterward we
  /// check Decl's name against list of current and above scopes before adding
  /// that name to identifiers defined in current scope.
  bool VisitCXXMethodOrFieldDecl(const clang::DeclaratorDecl *DD);

  /// Found new EnumDecl, check all scopes if it contains similar identifier.
  /// If this is scoped enum then we make a new scope.
  bool VisitEnumDecl(const clang::EnumDecl *ED) override;

  /// Checks new EnumConstantDecl against previously declared identifiers.
  bool VisitEnumConstantDecl(const clang::EnumConstantDecl *ECD) override;

  /// Variables declared inside typedef are not visible to outside scopes.
  bool VisitTypedefDecl(const clang::TypedefDecl *TD) override;

  /// Each namespace definition is a seperate scope for itself. Checks the name
  /// of the namespace against previously declared namespace identifiers.
  bool VisitNamespaceDecl(const clang::NamespaceDecl *ND) override;

  /// Check if variable is local or global, check against previous declarations
  /// and add new Decl to appropriate list.
  bool VisitVarDecl(const clang::VarDecl *VD) override;

  /// Compound statements represent a block of code.
  bool VisitCompoundStmt(const clang::CompoundStmt *CS) override;

  /// Variables declared inside initialization of for statements are not
  /// visible to outside scopes.
  bool VisitForStmt(const clang::ForStmt *FS) override;

  /// Variables declared inside while statements are not visible to outside
  /// scopes.
  bool VisitWhileStmt(const clang::WhileStmt *WS) override;

  /// Variables declared inside do-while statements are not visible to outside
  /// scopes.
  bool VisitDoStmt(const clang::DoStmt *DS) override;

  /// Variables declared inside if statements are not visible to outside
  /// scopes.
  bool VisitIfStmt(const clang::IfStmt *IS) override;

  /// Variables declared inside lambda expression are not visiable to outside
  /// scopes.
  bool VisitLambdaExpr(const clang::LambdaExpr *LE) override;

private:
  /// Each block of code represents new range.
  bool VisitCodeBlock(const clang::Stmt *S);

  inline IdentifierVector &getGlobalVarIdentifiers();
  inline IdentifierVector &getGlobalTypeIdentifiers();
  inline IdentifierVector &getLocalVarIdentifiers();
  inline IdentifierVector &getLocalTypeIdentifiers();
  inline IdentifierVector &getLocalNamespaceIdentifiers();

  /// Update given list with new name and location.
  void addToIdentifierVector(IdentifierVector &Vec, const std::string &Name,
                             const clang::SourceLocation &Loc);

  /// Update given set with new name.
  void addToIdentifierSet(IdentifierSet &Set, const std::string &Name);

  /// Check if list already contains same identifier. Used for forward checking
  /// if class or function is forward delcarated.
  bool containsSameId(const IdentifierSet &Set, const std::string &Id) const;

  /// Check if list contains typographically similar identifier and report
  /// a warning.
  bool checkIdentifierVector(const IdentifierVector &Vec, const std::string &Id,
                             const clang::SourceLocation &Loc);

  /// Checks if a method has "override" or "final" attribute.
  inline bool isMethodMarkedOverrideOrFinal(const clang::Decl *D);

  /// Checks given name against all methods and fields for all subclasses.
  bool checkSubClassIndentifiers(const clang::CXXRecordDecl *CRD,
                                 const std::string &Name,
                                 const clang::SourceLocation &Loc);

  /// Moves the string iterator pass all underscores.
  void skipUnderscores(std::string::const_iterator &It,
                       const std::string &Str) const;

  /// Checks if characters are similar as described by MISRA C++ 2008 rules.
  bool similarCharacters(char C1, char C2) const;

  /// Checks if characters are similar as described by MISRA C++ 2008 rules.
  bool similarCharacterGroups(char C10, char C11, char C2) const;

  /// Compare if two string are typographically unambiguous as described by
  /// MISRA C++ 2008 rules.
  bool typographicallySimilar(const std::string &S1,
                              const std::string &S2) const;
};

typedef std::map<std::string, clang::SourceLocation> TrackingSet;

/// [A2-10-6] A class or enumeration name shall not be hidden by a variable,
/// function or enumerator declaration in the same scope.
class ShadowClassOrEnumVisitor : public LexicalRulesVisitorInterface {
private:
  clang::DiagnosticsEngine &DE;
  const AutocheckContext &Context;

  /// By using qualified names (such as qual1::qual2::name) we can use same
  /// identifiers sets on all scope levels.
  TrackingSet UsedIdentifiers;
  TrackingSet UsedClasses;
  TrackingSet UsedEnums;

public:
  explicit ShadowClassOrEnumVisitor(clang::DiagnosticsEngine &DE,
                                    const AutocheckContext &Context);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitFunctionDecl(const clang::FunctionDecl *FD) override;
  bool VisitVarDecl(const clang::VarDecl *VD) override;
  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *D) override;
  bool VisitEnumDecl(const clang::EnumDecl *D) override;
  bool VisitEnumConstantDecl(const clang::EnumConstantDecl *D) override;

private:
  /// Compare current name against all prevous ones and report a warning if the
  /// same one is found.
  bool checkIfNameIsHidden(const clang::NamedDecl *ND,
                           const TrackingSet &NameSet) const;

  /// Add current name to the set of identifiers.
  void addToSet(const clang::NamedDecl *ND, TrackingSet &NameSet);
};

/// [A3-9-1] Fixed width integer types from <cstdint>, indicating the size and
/// signedness, shall be used in place of the basic numerical types.
class FixedWidthIntegerTypesVisitor : public LexicalRulesVisitorInterface {
  clang::DiagnosticsEngine &DE;
  const AutocheckContext &Context;
  clang::SourceLocation PreviousWarningLocation;
  clang::SourceLocation MainReturnTypeLoc;

public:
  explicit FixedWidthIntegerTypesVisitor(clang::DiagnosticsEngine &DE,
                                         const AutocheckContext &Context);
  static bool isFlagPresent(const AutocheckContext &Context);

  /// Visits all types except ones in enum declarations.
  bool VisitTypeLoc(const clang::TypeLoc &TL) override;
  /// Visits enum declarations in order to find types which are not visited
  /// by VisitTypeLoc.
  bool VisitEnumDecl(const clang::EnumDecl *ED) override;
  /// Visits FunctionDecl in order to find location of return type of main
  /// and suppress diagnostics for it.
  bool VisitFunctionDecl(const clang::FunctionDecl *FD) override;

private:
  /// Checks if type is basic numerical type.
  bool checkType(const clang::Type *T, const clang::SourceLocation &Loc);
};

/// [M6-2-3] Before preprocessing, a null statement shall only occur on a line
/// by itself; it may be followed by a comment, provided that the first
/// character following the null statement is a white-space character.
class NullStmtNotAloneVisitor : public LexicalRulesVisitorInterface {
  clang::DiagnosticsEngine &DE;
  const AutocheckContext &Context;
  clang::ASTContext &AC;
  clang::SourceManager &SM;
  const clang::LangOptions &LO;

public:
  explicit NullStmtNotAloneVisitor(clang::DiagnosticsEngine &DE,
                                   const AutocheckContext &Context,
                                   clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitNullStmt(const clang::NullStmt *NS) override;
};

/// [M7-3-6] Using-directives and using-declarations (excluding class scope or
/// function scope using-declarations) shall not be used in header files.
class UsingInsideHeaderVisitor : public LexicalRulesVisitorInterface {
  clang::DiagnosticsEngine &DE;
  const AutocheckContext &Context;
  llvm::SmallVector<clang::Decl *, 8> ScopeStack;

public:
  explicit UsingInsideHeaderVisitor(clang::DiagnosticsEngine &DE,
                                    const AutocheckContext &Context);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool PreTraverseDecl(clang::Decl *D) override;
  bool PostTraverseDecl(clang::Decl *D) override;

  bool VisitUsingDirectiveDecl(const clang::UsingDirectiveDecl *UDD) override;
  bool VisitUsingDecl(const clang::UsingDecl *UD) override;
};

/// [A7-1-7] Each expression statement and identifier declaration shall be
/// placed on a separate line.
class SeparateLineStatementVisitor : public LexicalRulesVisitorInterface {
  clang::DiagnosticsEngine &DE;
  const AutocheckContext &Context;
  clang::ASTContext &AC;
  clang::SourceManager &SM;

  clang::SourceRange previousExprOrStmtRange;
  clang::SourceLocation previousWarning;

public:
  explicit SeparateLineStatementVisitor(clang::DiagnosticsEngine &DE,
                                        const AutocheckContext &Context,
                                        clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitDeclStmt(const clang::DeclStmt *DS) override;
  bool VisitForStmt(const clang::ForStmt *FS) override;
  bool VisitIfStmt(const clang::IfStmt *IS) override;
  bool VisitCXXForRangeStmt(const clang::CXXForRangeStmt *CFRS) override;
  bool VisitDoStmt(const clang::DoStmt *DS) override;
  bool VisitWhileStmt(const clang::WhileStmt *WS) override;
  bool VisitStmt(const clang::Stmt *S) override;
  bool VisitDecl(const clang::Decl *S) override;

  bool diagnoseSameLine(const clang::SourceLocation &Prev,
                        const clang::SourceLocation &Curr);
  clang::SourceLocation getBodyStart(const clang::Stmt *Body);
};

/// [A5-1-3] Parameter list (possibly empty) shall be included in every lambda
/// expression.
class LambdaDeclaratorVisitor : public LexicalRulesVisitorInterface {
  clang::DiagnosticsEngine &DE;
  clang::ASTContext &AC;

public:
  explicit LambdaDeclaratorVisitor(clang::DiagnosticsEngine &DE,
                                   clang::ASTContext &AC);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitLambdaExpr(const clang::LambdaExpr *LE) override;
};

// [M6-4-1] An if ( condition ) construct shall be followed by a compound
// statement. The else keyword shall be followed by either a compound statement,
// or another if statement.
class IfElseCompoundStmtVisitor : public LexicalRulesVisitorInterface {
  clang::DiagnosticsEngine &DE;

public:
  explicit IfElseCompoundStmtVisitor(clang::DiagnosticsEngine &DE);
  static bool isFlagPresent(const AutocheckContext &Context);

  bool VisitIfStmt(const clang::IfStmt *IS) override;
};

/// Main visitor for lexical rules. Makes an instance of every class that
/// implement a LexicalRulesVisitorInterface if appropriate flag is found. Runs
/// all Lexical Rule Visitors with one AST traversal.
class LexicalRulesVisitor
    : public clang::RecursiveASTVisitor<LexicalRulesVisitor> {
  clang::DiagnosticsEngine &DE;

  std::forward_list<std::unique_ptr<LexicalRulesVisitorInterface>> AllVisitors;

public:
  explicit LexicalRulesVisitor(clang::DiagnosticsEngine &DE,
                               clang::ASTContext &AC);
  void run(clang::TranslationUnitDecl *TUD);

  bool TraverseDecl(clang::Decl *D);

  bool VisitIntegerLiteral(const clang::IntegerLiteral *IL);
  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD);
  bool VisitTypedefDecl(const clang::TypedefDecl *TD);
  bool VisitNamespaceDecl(const clang::NamespaceDecl *ND);
  bool VisitVarDecl(const clang::VarDecl *VD);
  bool VisitFunctionDecl(const clang::FunctionDecl *FD);
  bool VisitFieldDecl(const clang::FieldDecl *FD);
  bool VisitCXXMethodDecl(const clang::CXXMethodDecl *CMD);
  bool VisitEnumDecl(const clang::EnumDecl *D);
  bool VisitEnumConstantDecl(const clang::EnumConstantDecl *ECD);
  bool VisitCompoundStmt(const clang::CompoundStmt *CS);
  bool VisitForStmt(const clang::ForStmt *FS);
  bool VisitWhileStmt(const clang::WhileStmt *WS);
  bool VisitDoStmt(const clang::DoStmt *DS);
  bool VisitIfStmt(const clang::IfStmt *IS);
  bool VisitLambdaExpr(const clang::LambdaExpr *LE);
  bool VisitNullStmt(const clang::NullStmt *NS);
  bool VisitTypeLoc(const clang::TypeLoc &TL);
  bool VisitTranslationUnitDecl(const clang::TranslationUnitDecl *TUD);
  bool VisitUsingDecl(const clang::UsingDecl *UD);
  bool VisitUsingDirectiveDecl(const clang::UsingDirectiveDecl *UDD);
  bool VisitDeclStmt(const clang::DeclStmt *DS);
  bool VisitCXXForRangeStmt(const clang::CXXForRangeStmt *CFRS);
  bool VisitStmt(const clang::Stmt *S);
  bool VisitDecl(const clang::Decl *D);
};

} // namespace autocheck

#endif
