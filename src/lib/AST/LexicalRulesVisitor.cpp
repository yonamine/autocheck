//===--- LexicalRulesVisitor.cpp - Visitor for lexer related checks -------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the LexicalRulesVisitor class and all classes derived
// from LexicalRulesVisitorInterface.
//
//===----------------------------------------------------------------------===//

#include "AST/LexicalRulesVisitor.h"
#include "Diagnostics/AutocheckDiagnostic.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/Basic/ExceptionSpecificationType.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/Syntax/Tokens.h"
#include "llvm/Support/Casting.h"

namespace autocheck {

/*LexicalRulesVisitorInterface*/

LexicalRulesVisitorInterface::~LexicalRulesVisitorInterface() {}

using LVI = LexicalRulesVisitorInterface;
bool LVI::PreTraverseDecl(clang::Decl *) { return true; }
bool LVI::PostTraverseDecl(clang::Decl *) { return true; }
bool LVI::VisitIntegerLiteral(const clang::IntegerLiteral *) { return true; }
bool LVI::VisitCXXRecordDecl(const clang::CXXRecordDecl *) { return true; }
bool LVI::VisitTypedefDecl(const clang::TypedefDecl *) { return true; }
bool LVI::VisitVarDecl(const clang::VarDecl *) { return true; }
bool LVI::VisitNamespaceDecl(const clang::NamespaceDecl *) { return true; }
bool LVI::VisitFunctionDecl(const clang::FunctionDecl *) { return true; }
bool LVI::VisitFieldDecl(const clang::FieldDecl *) { return true; }
bool LVI::VisitCXXMethodDecl(const clang::CXXMethodDecl *) { return true; }
bool LVI::VisitEnumDecl(const clang::EnumDecl *) { return true; }
bool LVI::VisitEnumConstantDecl(const clang::EnumConstantDecl *) {
  return true;
}
bool LVI::VisitCompoundStmt(const clang::CompoundStmt *) { return true; }
bool LVI::VisitForStmt(const clang::ForStmt *) { return true; }
bool LVI::VisitWhileStmt(const clang::WhileStmt *) { return true; }
bool LVI::VisitDoStmt(const clang::DoStmt *) { return true; }
bool LVI::VisitIfStmt(const clang::IfStmt *) { return true; }
bool LVI::VisitLambdaExpr(const clang::LambdaExpr *) { return true; }
bool LVI::VisitNullStmt(const clang::NullStmt *) { return true; }
bool LVI::VisitTypeLoc(const clang::TypeLoc &) { return true; }
bool LVI::VisitTranslationUnitDecl(const clang::TranslationUnitDecl *) {
  return true;
}
bool LVI::VisitUsingDecl(const clang::UsingDecl *) { return true; }
bool LVI::VisitUsingDirectiveDecl(const clang::UsingDirectiveDecl *) {
  return true;
}
bool LVI::VisitDeclStmt(const clang::DeclStmt *DS) { return true; }
bool LVI::VisitCXXForRangeStmt(const clang::CXXForRangeStmt *CFRS) {
  return true;
}
bool LVI::VisitStmt(const clang::Stmt *S) { return true; }
bool LVI::VisitDecl(const clang::Decl *D) { return true; }

/* UnsignedLiterals */

UnsignedLiterals::UnsignedLiterals(clang::DiagnosticsEngine &DE,
                                   const AutocheckContext &Context,
                                   clang::ASTContext &ASTCtx)
    : DE(DE), Context(Context), ASTCtx(ASTCtx) {}

bool UnsignedLiterals::VisitIntegerLiteral(const clang::IntegerLiteral *IL) {
  clang::SourceLocation Loc = IL->getBeginLoc();

  // Checks if literal is marked as unsigned.
  if (IL->getType()->isUnsignedIntegerOrEnumerationType())
    return true;

  // Checks if literal is not octal or hexadecimal.
  if (!isIntegerLiteralOctalorHexadecimal(IL))
    return true;

  const auto &Parents = ASTCtx.getParents(*IL);
  if (Parents.size() != 1)
    return true;

  // Checks if this literal is being cast.
  if (const clang::ImplicitCastExpr *ICE =
          Parents[0].get<clang::ImplicitCastExpr>()) {

    // Checks if this is not integer to integer cast.
    if (ICE->getCastKind() != clang::CK_IntegralCast)
      return true;

    // Check if this is signed integer to signed integer cast.
    if (IL->getType()->isSignedIntegerOrEnumerationType() ==
        ICE->getType()->isSignedIntegerOrEnumerationType())
      return true;

    const auto &GrandParents = ASTCtx.getParents(*ICE);
    if (GrandParents.size() != 1)
      return true;

    // Checks if this is explicit cast: CXXStaticCastExpr or CStyleCastExpr.
    if (GrandParents[0].get<clang::ExplicitCastExpr>() != nullptr)
      return true;

    // Signed literal is used as unsigned.
    return !AutocheckDiagnostic::reportWarning(
                DE, Loc, AutocheckWarnings::unsignedLiteralSuffix)
                .limitReached();
  }
  return true;
}

bool UnsignedLiterals::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::unsignedLiteralSuffix);
}

bool UnsignedLiterals::isIntegerLiteralOctalorHexadecimal(
    const clang::IntegerLiteral *IL) const {
  const char FirstChar =
      DE.getSourceManager().getCharacterData(IL->getBeginLoc())[0];
  const char SecondChar =
      DE.getSourceManager().getCharacterData(IL->getBeginLoc())[1];
  return (FirstChar == '0' && SecondChar != 'b' && SecondChar != 'B');
}

/*SimilarIdentifiersVisitor::IdScopes*/

SimilarIdentifiersVisitor::IdScopes::IdScopes(clang::SourceManager &SrcMgr)
    : isBefore(SrcMgr) {
  addNewScopeLevel(clang::SourceRange());
}

void SimilarIdentifiersVisitor::IdScopes::addNewScopeLevel(
    clang::SourceRange Range) {
  ScopeVector.push_back(ScopeData(Range));
}

void SimilarIdentifiersVisitor::IdScopes::update(
    const clang::SourceLocation &CurrentLocation) {
  while (!ScopeVector.empty() &&
         !isInSourceRange(getLastScope().getRange(), CurrentLocation))
    ScopeVector.pop_back();
}

bool SimilarIdentifiersVisitor::IdScopes::isInSourceRange(
    const clang::SourceLocation &BeginLoc, const clang::SourceLocation &EndLoc,
    const clang::SourceLocation &Location) const {
  if (BeginLoc.isInvalid() || EndLoc.isInvalid())
    return true; // Global (top level) range is set to be invalid.
  if (isBefore(Location, BeginLoc))
    return false;
  if (isBefore(EndLoc, Location))
    return false;
  return true;
}

inline bool SimilarIdentifiersVisitor::IdScopes::isInSourceRange(
    const clang::SourceRange &Range,
    const clang::SourceLocation &Location) const {
  return isInSourceRange(Range.getBegin(), Range.getEnd(), Location);
}

/*SimilarIdentifiersVisitor*/

SimilarIdentifiersVisitor::SimilarIdentifiersVisitor(
    clang::DiagnosticsEngine &DE, const AutocheckContext &Context)
    : DE(DE), Context(Context), SM(DE.getSourceManager()), Scopes(SM) {}

bool SimilarIdentifiersVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::similarIdentifiers);
}

bool SimilarIdentifiersVisitor::VisitFunctionDecl(
    const clang::FunctionDecl *FD) {
  if (llvm::dyn_cast_if_present<clang::CXXMethodDecl>(FD))
    return true;
  const std::string &FunctionName = FD->getNameAsString();
  const clang::SourceLocation &FunctionLoc = FD->getBeginLoc();
  const clang::SourceRange &FunctionRange = FD->getSourceRange();
  Scopes.update(FunctionLoc);
  // Skip names of functions that overload previously declared.
  if (containsSameId(Scopes.getGlobalScope().getOverloadingIdentifiers(),
                     FunctionName)) {
    Scopes.addNewScopeLevel(FunctionRange);
    return true;
  }
  // Remeber used names for checking if it is overloading.
  addToIdentifierSet(Scopes.getGlobalScope().getOverloadingIdentifiers(),
                     FunctionName);
  // Skip names of functions that have been previously forward declared.
  if (!containsSameId(Scopes.getGlobalScope().getForwardDeclarations(),
                      FunctionName)) {
    if (!checkIdentifierVector(getGlobalTypeIdentifiers(), FunctionName,
                               FunctionLoc))
      return false;
    addToIdentifierVector(getGlobalTypeIdentifiers(), FunctionName,
                          FunctionLoc);
  }
  // If this is a forward declaration that has a later definition then
  // that later definition will have same identifier and should not be
  // treated as typographically similar.
  if (!FD->isThisDeclarationADefinition())
    Scopes.getGlobalScope().getForwardDeclarations().insert(FunctionName);
  Scopes.addNewScopeLevel(FunctionRange);
  return true;
}

bool SimilarIdentifiersVisitor::VisitCXXRecordDecl(
    const clang::CXXRecordDecl *D) {
  const std::string &RecordName = D->getNameAsString();
  const clang::SourceLocation &RecordLoc = D->getBeginLoc();
  const clang::SourceRange &RecordRange = D->getSourceRange();
  Scopes.update(RecordLoc);
  // Skip names of classes that have been previously forward declared.
  if (!containsSameId(Scopes.getLastScope().getForwardDeclarations(),
                      RecordName)) {
    if (!checkIdentifierVector(getGlobalTypeIdentifiers(), RecordName,
                               RecordLoc))
      return false;
    addToIdentifierVector(getGlobalTypeIdentifiers(), RecordName, RecordLoc);
  }
  // If this is a forward declaration that has a later definition then that
  // later definition will have same identifier and should not be treated as
  // typographically similar.
  if (!D->isThisDeclarationADefinition() && D->hasDefinition())
    addToIdentifierSet(Scopes.getLastScope().getForwardDeclarations(),
                       RecordName);
  Scopes.addNewScopeLevel(RecordRange);
  // Constructors use same name as class name (so we can treat the same way
  // as if it is an overloading method).
  addToIdentifierSet(Scopes.getLastScope().getOverloadingIdentifiers(),
                     RecordName);
  return true;
}

bool SimilarIdentifiersVisitor::VisitFieldDecl(const clang::FieldDecl *D) {
  if (!VisitCXXMethodOrFieldDecl(D))
    return false;
  // Skip ObjC types.
  if (llvm::dyn_cast_if_present<clang::ObjCIvarDecl>(D))
    return true;
  // Check subclasses for similar names.
  if (const clang::CXXRecordDecl *RD =
          llvm::dyn_cast_or_null<clang::CXXRecordDecl>(D->getParent()))
    return checkSubClassIndentifiers(RD, D->getNameAsString(),
                                     D->getBeginLoc());
  return true;
}

bool SimilarIdentifiersVisitor::VisitCXXMethodDecl(
    const clang::CXXMethodDecl *D) {
  const std::string &DeclName = D->getNameAsString();
  const clang::SourceLocation &DeclLoc = D->getBeginLoc();
  const clang::SourceRange &DeclRange = D->getSourceRange();
  Scopes.update(DeclLoc);
  // Ignore methods that overload c++ operators.
  if (D->isOverloadedOperator()) {
    Scopes.addNewScopeLevel(DeclRange);
    return true;
  }
  // Ignore methods that overload other methods.
  if (containsSameId(Scopes.getLastScope().getOverloadingIdentifiers(),
                     DeclName)) {
    Scopes.addNewScopeLevel(DeclRange);
    return true;
  }
  addToIdentifierSet(Scopes.getLastScope().getOverloadingIdentifiers(),
                     DeclName);
  if (!VisitCXXMethodOrFieldDecl(D))
    return false;
  // Check subclasses for similar names but skip methods that override.
  if (!isMethodMarkedOverrideOrFinal(D))
    if (!checkSubClassIndentifiers(D->getParent(), DeclName, DeclLoc))
      return false;
  Scopes.addNewScopeLevel(DeclRange);
  return true;
}

bool SimilarIdentifiersVisitor::VisitCXXMethodOrFieldDecl(
    const clang::DeclaratorDecl *D) {
  const std::string &DeclName = D->getNameAsString();
  const clang::SourceLocation &DeclLoc = D->getBeginLoc();
  Scopes.update(DeclLoc);
  if (Scopes.empty())
    return true;
  for (auto &e : Scopes)
    if (!checkIdentifierVector(e.getLocalVarIdentifiers(), DeclName, DeclLoc))
      return false;
  addToIdentifierVector(getLocalVarIdentifiers(), DeclName, DeclLoc);
  return true;
}

bool SimilarIdentifiersVisitor::VisitEnumDecl(const clang::EnumDecl *D) {
  const std::string &EnumName = D->getNameAsString();
  if (EnumName.empty())
    return true;
  const clang::SourceLocation &EnumLoc = D->getBeginLoc();
  const clang::SourceRange &EnumRange = D->getSourceRange();
  Scopes.update(EnumLoc);
  for (auto &e : Scopes)
    if (!checkIdentifierVector(e.getLocalTypeIdentifiers(), EnumName, EnumLoc))
      return false;
  addToIdentifierVector(getLocalTypeIdentifiers(), EnumName, EnumLoc);
  if (D->isScoped())
    Scopes.addNewScopeLevel(EnumRange);
  return true;
}

bool SimilarIdentifiersVisitor::VisitEnumConstantDecl(
    const clang::EnumConstantDecl *D) {
  const std::string &EnumConstantName = D->getNameAsString();
  const clang::SourceLocation &EnumLoc = D->getBeginLoc();
  Scopes.update(EnumLoc);
  for (auto &e : Scopes)
    if (!checkIdentifierVector(e.getLocalVarIdentifiers(), EnumConstantName,
                               EnumLoc))
      return false;
  addToIdentifierVector(getLocalVarIdentifiers(), EnumConstantName, EnumLoc);
  return true;
}

bool SimilarIdentifiersVisitor::VisitTypedefDecl(const clang::TypedefDecl *D) {
  const std::string &TypedefName = D->getNameAsString();
  const clang::SourceLocation &TypedefLoc = D->getBeginLoc();
  const clang::SourceRange &TypedefRange = D->getSourceRange();
  Scopes.update(TypedefLoc);
  if (!checkIdentifierVector(getGlobalTypeIdentifiers(), TypedefName,
                             TypedefLoc))
    return false;
  addToIdentifierVector(getGlobalTypeIdentifiers(), TypedefName, TypedefLoc);
  Scopes.addNewScopeLevel(TypedefRange);
  return true;
}

bool SimilarIdentifiersVisitor::VisitNamespaceDecl(
    const clang::NamespaceDecl *D) {
  const std::string &NamespaceName = D->getNameAsString();
  if (NamespaceName.empty())
    return true;
  const clang::SourceLocation &NamespaceLoc = D->getBeginLoc();
  const clang::SourceRange &NamespaceRange = D->getSourceRange();
  Scopes.update(NamespaceLoc);
  for (auto &e : Scopes)
    if (!checkIdentifierVector(e.getLocalNamespaceIdentifiers(), NamespaceName,
                               NamespaceLoc))
      return false;
  addToIdentifierVector(getLocalNamespaceIdentifiers(), NamespaceName,
                        NamespaceLoc);
  Scopes.addNewScopeLevel(NamespaceRange);
  return true;
}

bool SimilarIdentifiersVisitor::VisitVarDecl(const clang::VarDecl *VD) {
  const std::string &VarName = VD->getNameAsString();
  const clang::SourceLocation &VarLoc = VD->getBeginLoc();
  if (VarName == "") // Skip empty parameter names in function declarations.
    return true;
  Scopes.update(VarLoc);
  for (auto &e : Scopes)
    if (!checkIdentifierVector(e.getLocalVarIdentifiers(), VarName, VarLoc))
      return false;
  addToIdentifierVector(getLocalVarIdentifiers(), VarName, VarLoc);
  return true;
}

bool SimilarIdentifiersVisitor::VisitCompoundStmt(
    const clang::CompoundStmt *S) {
  return VisitCodeBlock(S);
}

bool SimilarIdentifiersVisitor::VisitForStmt(const clang::ForStmt *S) {
  return VisitCodeBlock(S);
}

bool SimilarIdentifiersVisitor::VisitWhileStmt(const clang::WhileStmt *S) {
  return VisitCodeBlock(S);
}

bool SimilarIdentifiersVisitor::VisitDoStmt(const clang::DoStmt *S) {
  return VisitCodeBlock(S);
}

bool SimilarIdentifiersVisitor::VisitIfStmt(const clang::IfStmt *S) {
  return VisitCodeBlock(S);
}

bool SimilarIdentifiersVisitor::VisitLambdaExpr(const clang::LambdaExpr *S) {
  return VisitCodeBlock(S);
}

bool SimilarIdentifiersVisitor::VisitCodeBlock(const clang::Stmt *S) {
  Scopes.update(S->getBeginLoc());
  Scopes.addNewScopeLevel(S->getSourceRange());
  return true;
}

inline IdentifierVector &SimilarIdentifiersVisitor::getGlobalVarIdentifiers() {
  return Scopes.getGlobalScope().getLocalVarIdentifiers();
}

inline IdentifierVector &SimilarIdentifiersVisitor::getGlobalTypeIdentifiers() {
  return Scopes.getGlobalScope().getLocalTypeIdentifiers();
}

inline IdentifierVector &SimilarIdentifiersVisitor::getLocalVarIdentifiers() {
  return Scopes.getLastScope().getLocalVarIdentifiers();
}

inline IdentifierVector &SimilarIdentifiersVisitor::getLocalTypeIdentifiers() {
  return Scopes.getLastScope().getLocalTypeIdentifiers();
}

inline IdentifierVector &
SimilarIdentifiersVisitor::getLocalNamespaceIdentifiers() {
  return Scopes.getLastScope().getLocalNamespaceIdentifiers();
}

void SimilarIdentifiersVisitor::addToIdentifierVector(
    IdentifierVector &Vec, const std::string &Name,
    const clang::SourceLocation &Loc) {
  Vec.push_back(std::make_pair(Name, Loc));
}

void SimilarIdentifiersVisitor::addToIdentifierSet(IdentifierSet &Set,
                                                   const std::string &Name) {
  Set.insert(Name);
}

bool SimilarIdentifiersVisitor::containsSameId(const IdentifierSet &Set,
                                               const std::string &Id) const {
  return Set.count(Id);
}

bool SimilarIdentifiersVisitor::checkIdentifierVector(
    const IdentifierVector &Vec, const std::string &Id,
    const clang::SourceLocation &Loc) {
  for (const auto &It : Vec) {
    if (typographicallySimilar(It.first, Id)) {
      bool stopVisitor = AutocheckDiagnostic::reportWarning(
                             DE, Loc, AutocheckWarnings::similarIdentifiers)
                             .limitReached();
      DE.Report(It.second, clang::diag::note_previous_definition);
      if (stopVisitor)
        return false;
      // Only one warning per identifier is reported.
      return true;
    }
  }
  return true;
}

inline bool
SimilarIdentifiersVisitor::isMethodMarkedOverrideOrFinal(const clang::Decl *D) {
  for (const auto &A : D->attrs())
    if (A->getKind() == clang::attr::Kind::Override ||
        A->getKind() == clang::attr::Kind::Final)
      return true;
  return false;
}

bool SimilarIdentifiersVisitor::checkSubClassIndentifiers(
    const clang::CXXRecordDecl *RD, const std::string &Name,
    const clang::SourceLocation &Loc) {
  for (clang::CXXRecordDecl::base_class_const_iterator
           BaseIt = RD->bases_begin(),
           BaseEnd = RD->bases_end();
       BaseIt != BaseEnd; ++BaseIt) {
    if (const clang::CXXRecordDecl *BaseClass =
            BaseIt->getType()->getAsCXXRecordDecl()) {
      for (clang::CXXRecordDecl::method_iterator It = BaseClass->method_begin(),
                                                 End = BaseClass->method_end();
           It != End; ++It) {
        if (typographicallySimilar((*It)->getNameAsString(), Name)) {
          bool stopVisitor = AutocheckDiagnostic::reportWarning(
                                 DE, Loc, AutocheckWarnings::similarIdentifiers)
                                 .limitReached();
          DE.Report((*It)->getBeginLoc(),
                    clang::diag::note_previous_definition);
          if (stopVisitor)
            return false;
        }
      }
      for (clang::RecordDecl::field_iterator It = BaseClass->field_begin(),
                                             End = BaseClass->field_end();
           It != End; ++It) {
        if (typographicallySimilar((*It)->getNameAsString(), Name)) {
          bool stopVisitor = AutocheckDiagnostic::reportWarning(
                                 DE, Loc, AutocheckWarnings::similarIdentifiers)
                                 .limitReached();
          DE.Report((*It)->getBeginLoc(),
                    clang::diag::note_previous_definition);
          if (stopVisitor)
            return false;
        }
      }
    }
  }
  return true;
}

void SimilarIdentifiersVisitor::skipUnderscores(std::string::const_iterator &It,
                                                const std::string &Str) const {
  auto PosCurr = std::distance(Str.begin(), It);
  auto PosNext = Str.find_first_not_of('_', PosCurr);
  if (PosNext != std::string::npos)
    std::advance(It, PosNext - PosCurr);
  else
    It = Str.end();
}

bool SimilarIdentifiersVisitor::similarCharacters(char C1, char C2) const {
  return (tolower(C1) == tolower(C2) ||
          (C1 == '1' && ((C2 == 'I') || (C2 == 'i') || (C2 == 'l'))) ||
          (C1 == 'l' && C2 == 'I') || (C1 == 'O' && C2 == '0') ||
          (C1 == 'S' && C2 == '5') || (C1 == 'Z' && C2 == '2') ||
          (C1 == 'n' && C2 == 'h') || (C1 == 'B' && C2 == '8'));
}

bool SimilarIdentifiersVisitor::similarCharacterGroups(char C10, char C11,
                                                       char C2) const {
  return (C10 == 'r' && C11 == 'n' && C2 == 'm');
}

bool SimilarIdentifiersVisitor::typographicallySimilar(
    const std::string &S1, const std::string &S2) const {
  if (!S1.compare(S2))
    return false;
  std::string::const_iterator I1 = S1.begin(), I2 = S2.begin(), E1 = S1.end(),
                              E2 = S2.end();
  for (; I1 != E1 && I2 != E2; ++I1, ++I2) {
    skipUnderscores(I1, S1);
    skipUnderscores(I2, S2);
    if (I1 == E1 || I2 == E2)
      break;
    if (similarCharacterGroups(*I1, *(I1 + 1), *I2))
      ++I1;
    else if (similarCharacterGroups(*I2, *(I2 + 1), *I1))
      ++I2;
    else if (!(similarCharacters(*I1, *I2) || similarCharacters(*I2, *I1)))
      return false;
  }
  skipUnderscores(I1, S1);
  skipUnderscores(I2, S2);
  return (I1 == E1 && I2 == E2);
}

/*ShadowClassOrEnumVisitor*/

ShadowClassOrEnumVisitor::ShadowClassOrEnumVisitor(
    clang::DiagnosticsEngine &DE, const AutocheckContext &Context)
    : DE(DE), Context(Context) {}

bool ShadowClassOrEnumVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::shadowClassOrEnum);
}

bool ShadowClassOrEnumVisitor::VisitFunctionDecl(
    const clang::FunctionDecl *FD) {
  if (!checkIfNameIsHidden(FD, UsedClasses))
    return false;
  if (!checkIfNameIsHidden(FD, UsedEnums))
    return false;
  addToSet(FD, UsedIdentifiers);
  return true;
}

bool ShadowClassOrEnumVisitor::VisitVarDecl(const clang::VarDecl *VD) {
  if (VD->getNameAsString().empty())
    return true;
  if (!checkIfNameIsHidden(VD, UsedClasses))
    return false;
  if (!checkIfNameIsHidden(VD, UsedEnums))
    return false;
  addToSet(VD, UsedIdentifiers);
  return true;
}

bool ShadowClassOrEnumVisitor::VisitCXXRecordDecl(
    const clang::CXXRecordDecl *D) {
  if (!checkIfNameIsHidden(D, UsedIdentifiers))
    return false;
  if (!checkIfNameIsHidden(D, UsedEnums))
    return false;
  addToSet(D, UsedClasses);
  return true;
}

bool ShadowClassOrEnumVisitor::VisitEnumDecl(const clang::EnumDecl *D) {
  if (!checkIfNameIsHidden(D, UsedIdentifiers))
    return false;
  if (!checkIfNameIsHidden(D, UsedClasses))
    return false;
  addToSet(D, UsedEnums);
  return true;
}

bool ShadowClassOrEnumVisitor::VisitEnumConstantDecl(
    const clang::EnumConstantDecl *D) {
  if (!checkIfNameIsHidden(D, UsedIdentifiers))
    return false;
  if (!checkIfNameIsHidden(D, UsedClasses))
    return false;
  addToSet(D, UsedEnums);
  return true;
}

bool ShadowClassOrEnumVisitor::checkIfNameIsHidden(const clang::NamedDecl *ND,
                                                   const TrackingSet &NameSet) {
  std::string Name = ND->getQualifiedNameAsString();
  clang::SourceLocation Location = ND->getBeginLoc();

  auto It = NameSet.find(Name);
  if (It != NameSet.end()) {
    bool stopVisitor = AutocheckDiagnostic::reportWarning(
                           DE, Location, AutocheckWarnings::shadowClassOrEnum)
                           .limitReached();
    DE.Report(It->second, clang::diag::note_previous_declaration);
    if (stopVisitor)
      return false;
  }

  return true;
}

void ShadowClassOrEnumVisitor::addToSet(const clang::NamedDecl *ND,
                                        TrackingSet &NameSet) {
  std::string Name = ND->getQualifiedNameAsString();
  clang::SourceLocation Location = ND->getBeginLoc();

  NameSet.insert(std::make_pair(Name, Location));
}

/* FixedWidthIntegerTypesVisitor */

FixedWidthIntegerTypesVisitor::FixedWidthIntegerTypesVisitor(
    clang::DiagnosticsEngine &DE, const AutocheckContext &Context)
    : DE(DE), Context(Context) {}

bool FixedWidthIntegerTypesVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::fixedWidthIntegerTypes);
}

bool FixedWidthIntegerTypesVisitor::VisitTypeLoc(const clang::TypeLoc &TL) {
  return checkType(TL.getTypePtr(), TL.getBeginLoc());
}

bool FixedWidthIntegerTypesVisitor::VisitEnumDecl(const clang::EnumDecl *ED) {
  if (ED->getIntegerTypeSourceInfo())
    return checkType(
        ED->getIntegerType().getTypePtr(),
        ED->getIntegerTypeSourceInfo()->getTypeLoc().getBeginLoc());
  return true;
}

bool FixedWidthIntegerTypesVisitor::VisitFunctionDecl(
    const clang::FunctionDecl *FD) {
  if (FD->isMain())
    MainReturnTypeLoc = FD->getTypeSourceInfo()->getTypeLoc().getBeginLoc();
  return true;
}

bool FixedWidthIntegerTypesVisitor::checkType(
    const clang::Type *T, const clang::SourceLocation &Loc) {
  if (!T->isBuiltinType())
    return true;

  if (!T->isIntegerType() || T->isBooleanType() || T->isWideCharType())
    return true;

  // Skip Elaborated, Typedef and Using types to avoid getting multiple warnings
  // for same type.
  if (llvm::dyn_cast_if_present<clang::ElaboratedType>(T) ||
      llvm::dyn_cast_if_present<clang::TypedefType>(T) ||
      llvm::dyn_cast_if_present<clang::UsingType>(T))
    return true;

  // Skip auto-deduced integer types.
  if (T->getContainedAutoType())
    return true;

  // Don't warn multiple times in a row for same type.
  // Example: int a,b,c;
  if (PreviousWarningLocation.isValid() && PreviousWarningLocation == Loc)
    return true;

  // Skip main return type int.
  if (MainReturnTypeLoc.isValid() && (Loc == MainReturnTypeLoc))
    return true;

  PreviousWarningLocation = Loc;
  return !AutocheckDiagnostic::reportWarning(
              DE, Loc, AutocheckWarnings::fixedWidthIntegerTypes)
              .limitReached();
}

/* NullStmtNotAloneVisitor */

NullStmtNotAloneVisitor::NullStmtNotAloneVisitor(
    clang::DiagnosticsEngine &DE, const AutocheckContext &Context,
    clang::ASTContext &AC)
    : DE(DE), Context(Context), AC(AC), SM(AC.getSourceManager()),
      LO(AC.getLangOpts()) {}

bool NullStmtNotAloneVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::nullStatementNotAlone);
}

bool NullStmtNotAloneVisitor::VisitNullStmt(const clang::NullStmt *NS) {
  // Calculate number of characters between null-statement and end-of-line.
  unsigned EOLOffset = 0;
  clang::SourceLocation NSEndLoc = NS->getEndLoc();
  if (NSEndLoc.isInvalid())
    return true;

  clang::SourceLocation NSEndLocWithOffset = NSEndLoc.getLocWithOffset(1);
  if (NSEndLocWithOffset.isInvalid())
    return true;

  clang::FullSourceLoc FSL1 = AC.getFullLoc(NSEndLocWithOffset);
  clang::FullSourceLoc FSL2 =
      AC.getFullLoc(NSEndLoc.getLocWithOffset(EOLOffset + 1));
  bool isInvalid1 = false;
  bool isInvalid2 = false;
  while (FSL1.getLineNumber(&isInvalid1) == FSL2.getLineNumber(&isInvalid2)) {
    if (isInvalid1 || isInvalid2)
      return true;
    ++EOLOffset;
    FSL2 = AC.getFullLoc(NSEndLoc.getLocWithOffset(EOLOffset + 1));
  }

  // Get null-statement's column number.
  const unsigned NSColumn =
      AC.getFullLoc(NS->getBeginLoc()).getSpellingColumnNumber();

  // Process characters before null-statement.
  clang::CharSourceRange RangeBeforeNullStmt;
  RangeBeforeNullStmt.setBegin(NSEndLoc.getLocWithOffset(-NSColumn + 1));
  RangeBeforeNullStmt.setEnd(NSEndLoc);
  bool Error = false;
  llvm::StringRef ContentBeforeNullStmt =
      clang::Lexer::getSourceText(RangeBeforeNullStmt, SM, LO, &Error);
  if (Error)
    return true;
  if (ContentBeforeNullStmt.trim().size() > 0) {
    if (AutocheckDiagnostic::reportWarning(
            DE, NS->getSemiLoc(), AutocheckWarnings::nullStatementNotAlone)
            .limitReached())
      return false;
  }

  // Process characters after null-statement.
  clang::CharSourceRange RangeAfterNullStmt;
  RangeAfterNullStmt.setBegin(NSEndLoc.getLocWithOffset(1));
  RangeAfterNullStmt.setEnd(NSEndLoc.getLocWithOffset(EOLOffset));
  llvm::StringRef ContentAfterNullStmt =
      clang::Lexer::getSourceText(RangeAfterNullStmt, SM, LO, &Error);
  if (Error)
    return true;
  const unsigned SubStrSize = ContentAfterNullStmt.size();
  if (SubStrSize > 0) {
    if (isspace(ContentAfterNullStmt[0]) && (SubStrSize > 1))
      if (ContentAfterNullStmt[1] == '/' &&
          (ContentAfterNullStmt[2] == '/' || ContentAfterNullStmt[2] == '*'))
        return true;
    return !AutocheckDiagnostic::reportWarning(
                DE, NS->getSemiLoc(), AutocheckWarnings::nullStatementNotAlone)
                .limitReached();
  }

  return true;
}

/* UsingInsideHeaderVisitor */

UsingInsideHeaderVisitor::UsingInsideHeaderVisitor(
    clang::DiagnosticsEngine &DE, const AutocheckContext &Context)
    : DE(DE), Context(Context) {}

bool UsingInsideHeaderVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::usingInsideHeader);
}

bool UsingInsideHeaderVisitor::VisitUsingDirectiveDecl(
    const clang::UsingDirectiveDecl *UDD) {
  const clang::SourceLocation &SL = UDD->getUsingLoc();
  if (!DE.getSourceManager().isInMainFile(SL)) {
    return !AutocheckDiagnostic::reportWarning(
                DE, SL, AutocheckWarnings::usingInsideHeader)
                .limitReached();
  }

  return true;
}

bool UsingInsideHeaderVisitor::VisitUsingDecl(const clang::UsingDecl *UD) {
  const clang::SourceLocation &SL = UD->getUsingLoc();
  if (ScopeStack.empty() && !DE.getSourceManager().isInMainFile(SL)) {
    return !AutocheckDiagnostic::reportWarning(
                DE, SL, AutocheckWarnings::usingInsideHeader)
                .limitReached();
  }

  return true;
}

bool UsingInsideHeaderVisitor::PreTraverseDecl(clang::Decl *D) {
  if (llvm::dyn_cast_if_present<clang::FunctionDecl>(D))
    ScopeStack.push_back(D);
  else if (const clang::CXXRecordDecl *CRD =
               llvm::dyn_cast_if_present<clang::CXXRecordDecl>(D)) {
    if (CRD->isClass())
      ScopeStack.push_back(D);
  }

  return true;
}

bool UsingInsideHeaderVisitor::PostTraverseDecl(clang::Decl *D) {
  if (!ScopeStack.empty() && D == ScopeStack.back())
    ScopeStack.pop_back(); // left scope of class or function D

  return true;
}

/* SeparateLineStatementVisitor */

SeparateLineStatementVisitor::SeparateLineStatementVisitor(
    clang::DiagnosticsEngine &DE, const AutocheckContext &Context,
    clang::ASTContext &AC)
    : DE(DE), Context(Context), AC(AC), SM(AC.getSourceManager()) {}

bool SeparateLineStatementVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::separateLineExpressionStatement);
}

bool SeparateLineStatementVisitor::VisitDeclStmt(const clang::DeclStmt *DS) {
  const auto &Parents = AC.getParents(*DS);
  const clang::SourceManager &SM = AC.getSourceManager();
  // Allow multiple declarations in the same line when they are inside of
  // initialization part of for loop
  if (!Parents.empty() && Parents[0].get<clang::ForStmt>()) {
    return true;
  }
  // Diagnose multiple declarations in the same line
  if (!DS->isSingleDecl()) {
    unsigned previousDeclLine = -1;
    for (const auto Decl : DS->getDeclGroup()) {
      if (SM.getSpellingLineNumber(Decl->getLocation()) == previousDeclLine) {
        previousWarning = Decl->getLocation();
        if (AutocheckDiagnostic::reportWarning(
                DE, Decl->getLocation(),
                AutocheckWarnings::separateLineExpressionStatement)
                .limitReached())
          return false;
        break;
      }
      previousDeclLine = SM.getSpellingLineNumber(Decl->getLocation());
    }
  }

  return true;
}

// Check whether the first statement in the for loop's body is at a proper line
bool SeparateLineStatementVisitor::VisitForStmt(const clang::ForStmt *FS) {
  clang::SourceLocation BodyStart = getBodyStart(FS->getBody());
  if (FS->getInc()) {
    clang::SourceLocation IncLoc = FS->getInc()->getEndLoc();
    if (!diagnoseSameLine(IncLoc, BodyStart)) {
      return false;
    }
  } else if (FS->getCond()) {
    clang::SourceLocation CondLoc = FS->getCond()->getEndLoc();
    if (!diagnoseSameLine(CondLoc, BodyStart)) {
      return false;
    }
  } else if (FS->getInit()) {
    clang::SourceLocation InitLoc = FS->getInit()->getEndLoc();
    if (!diagnoseSameLine(InitLoc, BodyStart)) {
      return false;
    }
  }

  return true;
}

bool SeparateLineStatementVisitor::VisitIfStmt(const clang::IfStmt *IS) {
  clang::SourceLocation ThenStart = getBodyStart(IS->getThen());
  if (!diagnoseSameLine(IS->getIfLoc(), ThenStart))
    return false;

  if (const clang::Stmt *Else = IS->getElse()) {
    // This will point to the location of if keyword within else-if statement or
    // to the location of the first statement within normal else statement
    clang::SourceLocation ElseStart = getBodyStart(Else);

    if (const clang::IfStmt *ElseIf =
            llvm::dyn_cast_if_present<clang::IfStmt>(Else)) {
      // else if statement
      clang::SourceLocation ElseIfBody = getBodyStart(ElseIf->getThen());
      if (!diagnoseSameLine(ElseStart, ElseIfBody))
        return false;
    } else {
      // normal else
      clang::SourceLocation ElseLoc = IS->getElseLoc();
      if (!diagnoseSameLine(ElseLoc, ElseStart))
        return false;
    }
  }

  return true;
}

bool SeparateLineStatementVisitor::VisitDoStmt(const clang::DoStmt *DS) {
  clang::SourceLocation BodyStart = getBodyStart(DS->getBody());
  clang::SourceLocation DoLoc = DS->getDoLoc();
  if (!diagnoseSameLine(DoLoc, BodyStart))
    return false;

  return true;
}

bool SeparateLineStatementVisitor::VisitWhileStmt(const clang::WhileStmt *WS) {
  clang::SourceLocation BodyStart = getBodyStart(WS->getBody());
  clang::SourceLocation CondLoc = WS->getCond()->getBeginLoc();
  if (!diagnoseSameLine(CondLoc, BodyStart))
    return false;

  return true;
}

bool SeparateLineStatementVisitor::VisitCXXForRangeStmt(
    const clang::CXXForRangeStmt *CFRS) {
  clang::SourceLocation BodyStart = getBodyStart(CFRS->getBody());
  clang::SourceLocation RParenLoc = CFRS->getRParenLoc();
  if (!diagnoseSameLine(RParenLoc, BodyStart))
    return false;

  return true;
}

bool SeparateLineStatementVisitor::VisitStmt(const clang::Stmt *S) {

  const clang::SourceLocation StmtBegin =
      SM.getExpansionRange(S->getBeginLoc()).getBegin();
  const clang::SourceLocation StmtEnd =
      SM.getExpansionRange(S->getEndLoc()).getEnd();

  const clang::SourceLocation BeginPreviousExprOrStmt =
      previousExprOrStmtRange.getBegin();
  const clang::SourceLocation EndPreviousExprOrStmt =
      previousExprOrStmtRange.getEnd();

  // Handle only top level expressions: some node's children are probably going
  // to be on the same line as the parent, but that shouldn't be reported as a
  // warning. For example, expression 1 + 2 + 3 is composed of two
  // BinaryOperator nodes which appear on the same line, but since we want to
  // allow this, only the nodes that have CompoundStmt as a parent should be
  // checked.
  const auto &Parents = AC.getParents(*S);
  if (!Parents.empty() && Parents[0].get<clang::CompoundStmt>()) {
    // Handle macro expansions
    if (SM.isMacroBodyExpansion(S->getBeginLoc()) ||
        SM.isMacroArgExpansion(S->getBeginLoc())) {
      // Allow macro expansions within previous expansions, expressions or
      // statements
      if (!SM.isPointWithin(StmtBegin, BeginPreviousExprOrStmt,
                            EndPreviousExprOrStmt)) {
        if (!diagnoseSameLine(EndPreviousExprOrStmt, StmtBegin))
          return false;
      }
      previousExprOrStmtRange = clang::SourceRange(StmtBegin, StmtEnd);
    } else if (SM.isInMainFile(StmtBegin) &&
               !llvm::dyn_cast_if_present<clang::CompoundStmt>(S)) {
      if (!diagnoseSameLine(EndPreviousExprOrStmt, StmtBegin) ||
          !diagnoseSameLine(BeginPreviousExprOrStmt, StmtBegin))
        return false;
      previousExprOrStmtRange = clang::SourceRange(StmtBegin, StmtEnd);
    }
  }
  return true;
}

bool SeparateLineStatementVisitor::VisitDecl(const clang::Decl *D) {

  // Process all declarations except the ones listed here. All nodes that have
  // DeclStmt as their parent are already processed in VisitDeclStmt method.
  const auto &Parents = AC.getParents(*D);
  if (!Parents.empty() && !Parents[0].get<clang::DeclStmt>() &&
      !llvm::dyn_cast_if_present<clang::NamespaceDecl>(D) &&
      !llvm::dyn_cast_if_present<clang::TranslationUnitDecl>(D) &&
      !llvm::dyn_cast_if_present<clang::ParmVarDecl>(D) &&
      !llvm::dyn_cast_if_present<clang::TemplateTypeParmDecl>(D) &&
      !llvm::dyn_cast_if_present<clang::TemplateDecl>(D) &&
      !llvm::dyn_cast_if_present<clang::NonTypeTemplateParmDecl>(D) &&
      !llvm::dyn_cast_if_present<clang::EmptyDecl>(D) &&
      !llvm::dyn_cast_if_present<clang::FriendDecl>(D)) {

    const clang::SourceLocation DeclBegin =
        SM.getExpansionLoc(D->getBeginLoc());
    const clang::SourceLocation DeclEnd = SM.getExpansionLoc(D->getEndLoc());

    const clang::SourceLocation BeginPreviousExprOrStmt =
        previousExprOrStmtRange.getBegin();
    const clang::SourceLocation EndPreviousExprOrStmt =
        previousExprOrStmtRange.getEnd();

    if (!diagnoseSameLine(EndPreviousExprOrStmt, DeclBegin) ||
        !diagnoseSameLine(BeginPreviousExprOrStmt, DeclBegin))
      return false;
    previousExprOrStmtRange = clang::SourceRange(DeclBegin, DeclEnd);
  }
  return true;
}

bool SeparateLineStatementVisitor::diagnoseSameLine(
    const clang::SourceLocation &Prev, const clang::SourceLocation &Curr) {
  // Don't report multiple warnings on the same line
  if (SM.getExpansionLineNumber(previousWarning) !=
          SM.getExpansionLineNumber(Curr) ||
      SM.getFileID(SM.getExpansionLoc(previousWarning)) !=
          SM.getFileID(SM.getExpansionLoc(Curr))) {
    if (SM.getExpansionLineNumber(Prev) == SM.getExpansionLineNumber(Curr) &&
        SM.getFileID(Prev) == SM.getFileID(Curr)) {
      previousWarning = Curr;
      if (AutocheckDiagnostic::reportWarning(
              DE, Curr, AutocheckWarnings::separateLineExpressionStatement)
              .limitReached())
        return false;
    }
  }
  return true;
}

clang::SourceLocation
SeparateLineStatementVisitor::getBodyStart(const clang::Stmt *Body) {
  clang::SourceLocation BodyStart;
  if (const clang::CompoundStmt *CompoundBody =
          llvm::dyn_cast_if_present<clang::CompoundStmt>(Body)) {
    if (!CompoundBody->body_empty()) {
      BodyStart = CompoundBody->body_front()->getBeginLoc();
    }
  } else if (!llvm::dyn_cast_if_present<clang::NullStmt>(Body)) {
    BodyStart = Body->getBeginLoc();
  }
  return BodyStart;
}

/* Implementation of LambdaDeclaratorVisitor */

LambdaDeclaratorVisitor::LambdaDeclaratorVisitor(clang::DiagnosticsEngine &DE,
                                                 clang::ASTContext &AC)
    : DE(DE), AC(AC) {}

bool LambdaDeclaratorVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::lambdaDeclaratorMissing);
}

bool LambdaDeclaratorVisitor::VisitLambdaExpr(const clang::LambdaExpr *LE) {
  // Note: LE->getCallOperator()->getParametersSourceRange() is invalid if there
  // are no parameters, no matter if the parentheses are present or not, so it
  // can't be used to check this rule.

  // If there are more than 0 parameters, parentheses are required. The only
  // case that needs checking is for empty parentheses.
  if (LE->getCallOperator()->getNumParams() > 0)
    return true;

  // Extract the code after the introducer and before the body.
  const clang::SourceManager &SM = DE.getSourceManager();
  clang::SourceRange DeclaratorRange(
      LE->getIntroducerRange().getEnd().getLocWithOffset(1),
      LE->getBody()->getBeginLoc());
  clang::syntax::FileRange DeclaratorFileRange(SM, DeclaratorRange.getBegin(),
                                               DeclaratorRange.getEnd());

  // If the range is too short, report the warning.
  if (DeclaratorFileRange.length() < 2) {
    return !AutocheckDiagnostic::reportWarning(
                DE, DeclaratorRange.getBegin(),
                AutocheckWarnings::lambdaDeclaratorMissing)
                .limitReached();
  }

  // Tokenize the declarator range.
  auto tokens = clang::syntax::tokenize(
      DeclaratorFileRange, DE.getSourceManager(), AC.getLangOpts());

  // Manually check for parentheses.
  if (tokens[0].kind() != clang::tok::l_paren ||
      tokens[1].kind() != clang::tok::r_paren ||
      (tokens.size() > 2 && tokens[2].kind() != clang::tok::arrow)) {
    return !AutocheckDiagnostic::reportWarning(
                DE, DeclaratorRange.getBegin(),
                AutocheckWarnings::lambdaDeclaratorMissing)
                .limitReached();
  }

  return true;
}

/* Implementation of IfElseCompoundStmtVisitor */

IfElseCompoundStmtVisitor::IfElseCompoundStmtVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool IfElseCompoundStmtVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::ifFollowedByCompoundStmt);
}

bool IfElseCompoundStmtVisitor::VisitIfStmt(const clang::IfStmt *IS) {
  // Check compound statement after if.
  if (!llvm::dyn_cast_if_present<clang::CompoundStmt>(IS->getThen())) {
    if (AutocheckDiagnostic::reportWarning(
            DE, IS->getIfLoc(), AutocheckWarnings::ifFollowedByCompoundStmt)
            .limitReached())
      return false;
  }

  // Check compound statement or if stmt after else.
  if (const clang::Stmt *Else = IS->getElse()) {
    if (!llvm::dyn_cast<clang::IfStmt>(Else) &&
        !llvm::dyn_cast<clang::CompoundStmt>(Else)) {
      if (AutocheckDiagnostic::reportWarning(
              DE, IS->getElseLoc(), AutocheckWarnings::ifFollowedByCompoundStmt)
              .limitReached())
        return false;
    }
  }

  return true;
}

/* LexicalRulesVisitor */

LexicalRulesVisitor::LexicalRulesVisitor(clang::DiagnosticsEngine &DE,
                                         clang::ASTContext &AC)
    : DE(DE) {
  const AutocheckContext &Context = AutocheckContext::Get();
  if (NullStmtNotAloneVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<NullStmtNotAloneVisitor>(DE, Context, AC));
  if (FixedWidthIntegerTypesVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<FixedWidthIntegerTypesVisitor>(DE, Context));
  if (ShadowClassOrEnumVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<ShadowClassOrEnumVisitor>(DE, Context));
  if (SimilarIdentifiersVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<SimilarIdentifiersVisitor>(DE, Context));
  if (UnsignedLiterals::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<UnsignedLiterals>(DE, Context, AC));
  if (UsingInsideHeaderVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<UsingInsideHeaderVisitor>(DE, Context));
  if (SeparateLineStatementVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<SeparateLineStatementVisitor>(DE, Context, AC));
  if (LambdaDeclaratorVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<LambdaDeclaratorVisitor>(DE, AC));
  if (IfElseCompoundStmtVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<IfElseCompoundStmtVisitor>(DE));
}

void LexicalRulesVisitor::run(clang::TranslationUnitDecl *TUD) {
  if (!AllVisitors.empty())
    this->TraverseDecl(TUD);
}

bool LexicalRulesVisitor::TraverseDecl(clang::Decl *D) {
  if (!D) // Can be null if code had errors.
    return true;

  clang::SourceLocation Loc = D->getBeginLoc();

  if (Loc.isInvalid() || appropriateHeaderLocation(DE, Loc)) {
    AllVisitors.remove_if(
        [D](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
          return !V->PreTraverseDecl(D);
        });

    RecursiveASTVisitor<LexicalRulesVisitor>::TraverseDecl(D);

    AllVisitors.remove_if(
        [D](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
          return !V->PostTraverseDecl(D);
        });
  }
  return true;
}

bool LexicalRulesVisitor::VisitIntegerLiteral(const clang::IntegerLiteral *IL) {
  AllVisitors.remove_if([IL](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitIntegerLiteral(IL);
  });
  return true;
}

bool LexicalRulesVisitor::VisitCXXRecordDecl(const clang::CXXRecordDecl *D) {
  AllVisitors.remove_if([D](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitCXXRecordDecl(D);
  });
  return true;
}

bool LexicalRulesVisitor::VisitTypedefDecl(const clang::TypedefDecl *D) {
  AllVisitors.remove_if([D](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitTypedefDecl(D);
  });
  return true;
}

bool LexicalRulesVisitor::VisitVarDecl(const clang::VarDecl *VD) {
  AllVisitors.remove_if([VD](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitVarDecl(VD);
  });
  return true;
}

bool LexicalRulesVisitor::VisitFunctionDecl(const clang::FunctionDecl *FD) {
  AllVisitors.remove_if([FD](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitFunctionDecl(FD);
  });
  return true;
}

bool LexicalRulesVisitor::VisitNamespaceDecl(const clang::NamespaceDecl *D) {
  AllVisitors.remove_if([D](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitNamespaceDecl(D);
  });
  return true;
}

bool LexicalRulesVisitor::VisitFieldDecl(const clang::FieldDecl *D) {
  AllVisitors.remove_if([D](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitFieldDecl(D);
  });
  return true;
}

bool LexicalRulesVisitor::VisitCXXMethodDecl(const clang::CXXMethodDecl *D) {
  AllVisitors.remove_if([D](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitCXXMethodDecl(D);
  });
  return true;
}

bool LexicalRulesVisitor::VisitEnumDecl(const clang::EnumDecl *D) {
  AllVisitors.remove_if([D](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitEnumDecl(D);
  });
  return true;
}

bool LexicalRulesVisitor::VisitEnumConstantDecl(
    const clang::EnumConstantDecl *D) {
  AllVisitors.remove_if([D](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitEnumConstantDecl(D);
  });
  return true;
}

bool LexicalRulesVisitor::VisitCompoundStmt(const clang::CompoundStmt *S) {
  AllVisitors.remove_if([S](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitCompoundStmt(S);
  });
  return true;
}

bool LexicalRulesVisitor::VisitForStmt(const clang::ForStmt *S) {
  AllVisitors.remove_if([S](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitForStmt(S);
  });
  return true;
}

bool LexicalRulesVisitor::VisitWhileStmt(const clang::WhileStmt *S) {
  AllVisitors.remove_if([S](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitWhileStmt(S);
  });
  return true;
}

bool LexicalRulesVisitor::VisitDoStmt(const clang::DoStmt *S) {
  AllVisitors.remove_if([S](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitDoStmt(S);
  });
  return true;
}

bool LexicalRulesVisitor::VisitIfStmt(const clang::IfStmt *S) {
  AllVisitors.remove_if([S](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitIfStmt(S);
  });
  return true;
}

bool LexicalRulesVisitor::VisitLambdaExpr(const clang::LambdaExpr *S) {
  AllVisitors.remove_if([S](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitLambdaExpr(S);
  });
  return true;
}

bool LexicalRulesVisitor::VisitNullStmt(const clang::NullStmt *NS) {
  AllVisitors.remove_if([NS](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitNullStmt(NS);
  });
  return true;
}

bool LexicalRulesVisitor::VisitTypeLoc(const clang::TypeLoc &TL) {
  if (TL.getBeginLoc().isInvalid())
    return true;
  AllVisitors.remove_if([TL](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitTypeLoc(TL);
  });
  return true;
}

bool LexicalRulesVisitor::VisitTranslationUnitDecl(
    const clang::TranslationUnitDecl *TUD) {
  AllVisitors.remove_if(
      [TUD](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
        return !V->VisitTranslationUnitDecl(TUD);
      });
  return true;
}

bool LexicalRulesVisitor::VisitUsingDecl(const clang::UsingDecl *UD) {
  AllVisitors.remove_if([UD](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitUsingDecl(UD);
  });
  return true;
}

bool LexicalRulesVisitor::VisitUsingDirectiveDecl(
    const clang::UsingDirectiveDecl *UDD) {
  AllVisitors.remove_if(
      [UDD](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
        return !V->VisitUsingDirectiveDecl(UDD);
      });
  return true;
}

bool LexicalRulesVisitor::VisitDeclStmt(const clang::DeclStmt *DS) {
  AllVisitors.remove_if([DS](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitDeclStmt(DS);
  });
  return true;
}

bool LexicalRulesVisitor::VisitCXXForRangeStmt(
    const clang::CXXForRangeStmt *CFRS) {
  AllVisitors.remove_if(
      [CFRS](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
        return !V->VisitCXXForRangeStmt(CFRS);
      });
  return true;
}

bool LexicalRulesVisitor::VisitDecl(const clang::Decl *D) {
  AllVisitors.remove_if([D](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitDecl(D);
  });
  return true;
}

bool LexicalRulesVisitor::VisitStmt(const clang::Stmt *S) {
  AllVisitors.remove_if([S](std::unique_ptr<LexicalRulesVisitorInterface> &V) {
    return !V->VisitStmt(S);
  });
  return true;
}

} // namespace autocheck
