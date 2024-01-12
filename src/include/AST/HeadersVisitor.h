//===--- HeadersVisitor.h - Visitor for header related checks -------------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file performs checks for rules related to headers by traversing the
// clang AST.
//
// This implements the following checks:
// - [A16-2-2] There shall be no unused include directives.
//
//===----------------------------------------------------------------------===//

#ifndef AST_HEADERS_VISITOR_H
#define AST_HEADERS_VISITOR_H

#include "AutocheckContext.h"
#include "Lex/AutocheckPPCallbacks.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "llvm/ADT/StringSet.h"
#include <forward_list>

namespace autocheck {

class AutocheckDiagnostic;

/// Common interface for all header related visitors.
class HeadersVisitorInterface {
public:
  virtual ~HeadersVisitorInterface();

  /// Custom hook function called after the traversal is done and visitor was
  /// not interrupted early (e.g. warning limit was reached).
  virtual void PostWork();

  /// Custom traversal function called for each Decl before it and its subtree
  /// are visited.
  virtual bool PreTraverseDecl(clang::Decl *D);
  /// Custom traversal function called for each Decl after it and its entire
  /// subtree have been visited.
  virtual bool PostTraverseDecl(clang::Decl *D);

  virtual bool VisitCallExpr(const clang::CallExpr *CE);
  virtual bool VisitCXXConstructExpr(const clang::CXXConstructExpr *CCE);
  virtual bool VisitCXXRecordDecl(const clang::CXXRecordDecl *D);
  virtual bool VisitDeclRefExpr(const clang::DeclRefExpr *DRE);
  virtual bool VisitEnumDecl(const clang::EnumDecl *ED);
  virtual bool VisitGNUNullExpr(const clang::GNUNullExpr *GN);
  virtual bool VisitTranslationUnitDecl(const clang::TranslationUnitDecl *TUD);
  virtual bool VisitTypedefDecl(const clang::TypedefDecl *D);
  virtual bool VisitTypeLoc(const clang::TypeLoc &TL);
  virtual bool VisitUsingDirectiveDecl(const clang::UsingDirectiveDecl *UDD);
  virtual bool VisitVarDecl(const clang::VarDecl *VD);
};

/// [A16-2-2] There shall be no unused include directives.
///
/// Checks through all global variables, functions, CXXRecords (classes,
/// structs, unions), typedefs, enums if they are used from any included file
/// (i.e. defined in that included file) and removes that file from given set
/// of includes.
class HeadersUnusedVisitor : public HeadersVisitorInterface {
  typedef std::map<std::string, clang::SourceLocation> TrackingSet;
  typedef std::map<const clang::IdentifierInfo *, const clang::SourceLocation>
      IdentifierMap;

  AutocheckDiagnostic &AD;
  IncludeInfo &IncludeData; /// Include data gathered by PPCallback.
  TrackingSet Typedefs;     /// Set of currently found typedefs
  IdentifierMap CXXRecords; /// Set of currently found CXXRecords (classes,
                            /// structs, unions)
  bool SystemSmart = false; /// -Wheaders-unused-system-smart option is used.
  bool SystemOff = false;   /// -Wheaders-unused-system-off option is used.
  unsigned DiagID;          /// Diag of the used option.
  llvm::StringSet<> SubincludeUsed; // Keeps track of headers that are not used
                                    // but one or more of it's #include
                                    // directives is.

public:
  explicit HeadersUnusedVisitor(AutocheckDiagnostic &AD,
                                const AutocheckContext &Context,
                                IncludeInfo &IncludeData);

  static bool isFlagPresent(const AutocheckContext &Context);

  /// Emits warnings for all remaining headers. Should be called after the
  /// traversal is done.
  void PostWork() override;

  bool VisitTranslationUnitDecl(const clang::TranslationUnitDecl *TUD) override;

  /// Finds used variables in main file of compilation unit and removes
  /// associated include file from the set of includes.
  bool VisitDeclRefExpr(const clang::DeclRefExpr *DRE) override;

  /// Finds used classes, structs and unions in main file of compilation unit
  /// and removes associated include file from the set of includes.
  bool VisitCXXConstructExpr(const clang::CXXConstructExpr *CCE) override;

  /// Finds used functions in main file of compilation unit and removes
  /// associated include file from the set of includes.
  bool VisitCallExpr(const clang::CallExpr *CE) override;

  /// Keeps track of all defined CXXRecordDecls (classes, structs, unions) that
  /// are not in main file of compilation unit. Also checks if there is a
  /// definition of a forward declaration form a header file and removes
  /// associated include file from the set of includes.
  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) override;

  /// Keeps track of all defined typedefs that are not in main file of
  /// compilation unit.
  bool VisitTypedefDecl(const clang::TypedefDecl *TD) override;

  /// Finds used typedefs or CXXRecordDecls (classes, structs, unions) and
  /// remove associated includes from the set of includes.
  bool VisitVarDecl(const clang::VarDecl *VD) override;

  /// Finds used using directives that refer to namespace and removes associated
  /// include file from the set of includes.
  bool VisitUsingDirectiveDecl(const clang::UsingDirectiveDecl *UDD) override;

  /// NULL is treated as a special node not a macro by AST so it will not be
  /// caught with other macros in AutosarPPCallback. When such a node is found
  /// remove associated include file (<cstddef>) from the set of includes.
  bool VisitGNUNullExpr(const clang::GNUNullExpr *GNE) override;

  /// Checks type of enum decl if it is a typedef declared in a header and
  /// removes associated includes from the set of includes.
  bool VisitEnumDecl(const clang::EnumDecl *ED) override;

  /// Finds used types that are defined as typedefs and removes associated
  /// include file from the set of includes.
  bool VisitTypeLoc(const clang::TypeLoc &TL) override;

private:
  /// Removes filename referenced by Loc from list of Includes. If list becomes
  /// empty, returns false so that the visitor can be stopped early.
  bool removeFromIncludes(const clang::SourceLocation &Loc);
  /// \copydoc removeFromIncludes
  bool removeFromIncludes(const llvm::StringRef Id);

  /// Removes type modifiers like '*' or '&' found with at the end of type name.
  void removeTypeModifiers(std::string &TypeName);

  /// Removes keyword for specific CXXRecord.
  void removeKeywords(std::string &TypeName);

  /// Checks if string Str starts with string Start.
  inline bool startsWith(const std::string &Str,
                         const std::string &Start) const;
};

/// Main visitor for header related checks. Makes an instance of every class
/// that implements HeadersVisitorInterface if appropriate flag is found. Runs
/// all Headers Visitors with one AST traversal.
class HeadersVisitor : public clang::RecursiveASTVisitor<HeadersVisitor> {
  std::forward_list<std::unique_ptr<HeadersVisitorInterface>> AllVisitors;

public:
  explicit HeadersVisitor(AutocheckDiagnostic &AD,
                          AutocheckPPCallbacks &Callbacks);
  void run(clang::TranslationUnitDecl *TUD);

  bool TraverseDecl(clang::Decl *D);

  bool VisitCallExpr(const clang::CallExpr *CE);
  bool VisitCXXConstructExpr(const clang::CXXConstructExpr *CCE);
  bool VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD);
  bool VisitDeclRefExpr(const clang::DeclRefExpr *DRE);
  bool VisitEnumDecl(const clang::EnumDecl *ED);
  bool VisitGNUNullExpr(const clang::GNUNullExpr *GNE);
  bool VisitTranslationUnitDecl(const clang::TranslationUnitDecl *TUD);
  bool VisitTypedefDecl(const clang::TypedefDecl *TD);
  bool VisitTypeLoc(const clang::TypeLoc &TL);
  bool VisitUsingDirectiveDecl(const clang::UsingDirectiveDecl *UDD);
  bool VisitVarDecl(const clang::VarDecl *VD);
};

} // namespace autocheck

#endif
