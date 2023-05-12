//===--- DeclarationsVisitor.cpp - Visitor for declaration related checks -===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the DeclarationsVisitor class and all classes derived
// from DeclarationsVisitorInterface.
//
//===----------------------------------------------------------------------===//

#include "AST/DeclarationsVisitor.h"

#include "AST/ASTHelper.h"
#include "Diagnostics/AutocheckDiagnostic.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/Basic/DiagnosticSema.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Sema/Sema.h"
#include <sstream>

namespace autocheck {

DeclarationsVisitorInterface::~DeclarationsVisitorInterface() {}

using DVI = DeclarationsVisitorInterface;
void DVI::PostWork() {}
bool DVI::PreTraverseDecl(clang::Decl *) { return true; }
bool DVI::PostTraverseDecl(clang::Decl *) { return true; }
bool DVI::PreTraverseStmt(clang::Stmt *) { return true; }
bool DVI::PostTraverseStmt(clang::Stmt *) { return true; }
bool DVI::VisitGCCAsmStmt(const clang::GCCAsmStmt *) { return true; }
bool DVI::VisitVarDecl(const clang::VarDecl *) { return true; }
bool DVI::VisitFunctionDecl(const clang::FunctionDecl *) { return true; }
bool DVI::VisitTypedefNameDecl(const clang::TypedefNameDecl *) { return true; }
bool DVI::VisitCXXMethodDecl(const clang::CXXMethodDecl *) { return true; }
bool DVI::VisitUnaryOperator(const clang::UnaryOperator *) { return true; }
bool DVI::VisitBinaryOperator(const clang::BinaryOperator *) { return true; }
bool DVI::VisitCallExpr(const clang::CallExpr *) { return true; }
bool DVI::VisitCXXRecordDecl(const clang::CXXRecordDecl *) { return true; }
bool DVI::VisitFieldDecl(const clang::FieldDecl *) { return true; }
bool DVI::VisitDeclRefExpr(const clang::DeclRefExpr *) { return true; }
bool DVI::VisitCXXConstructExpr(const clang::CXXConstructExpr *) {
  return true;
}
bool DVI::VisitCXXThrowExpr(const clang::CXXThrowExpr *) { return true; }
bool DVI::VisitCXXDeleteExpr(const clang::CXXDeleteExpr *) { return true; }
bool DVI::VisitTypedefDecl(const clang::TypedefDecl *) { return true; }
bool DVI::VisitFriendDecl(const clang::FriendDecl *) { return true; }
bool DVI::VisitEnumDecl(const clang::EnumDecl *) { return true; }
bool DVI::VisitNamespaceDecl(const clang::NamespaceDecl *) { return true; }
bool DVI::VisitUsingDirectiveDecl(const clang::UsingDirectiveDecl *) {
  return true;
}
bool DVI::VisitTranslationUnitDecl(const clang::TranslationUnitDecl *) {
  return true;
}

/* Implementation of ASMDeclarationUsedVisitor */

ASMDeclarationUsedVisitor::ASMDeclarationUsedVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool ASMDeclarationUsedVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::asmDeclarationUsed);
}

bool ASMDeclarationUsedVisitor::VisitGCCAsmStmt(const clang::GCCAsmStmt *GAS) {
  if (AutocheckDiagnostic::reportWarning(DE, GAS->getBeginLoc(),
                                         AutocheckWarnings::asmDeclarationUsed)
          .limitReached())
    return false;

  return true;
}

/* Implementation of UnusedGlobalTypedef */

UnusedGlobalTypedef::UnusedGlobalTypedef(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool UnusedGlobalTypedef::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::unusedTypedef);
}

void UnusedGlobalTypedef::PostWork() {
  for (auto *it : GlobalTypedefs) {
    if (!it->isReferenced()) {
      if (AutocheckDiagnostic::reportWarning(DE, it->getLocation(),
                                             AutocheckWarnings::unusedTypedef,
                                             0, it->getDeclName())
              .limitReached())
        break;
    }
  }
  GlobalTypedefs.clear();
}

bool UnusedGlobalTypedef::VisitTypedefNameDecl(
    const clang::TypedefNameDecl *TND) {
  if (TND->getDeclContext()->isFileContext() &&
      DE.getSourceManager().isInMainFile(TND->getBeginLoc()))
    GlobalTypedefs.push_back(TND);
  return true;
}

/* Implementation of VariadicFunctionUsedVisitor */

VariadicFunctionUsedVisitor::VariadicFunctionUsedVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool VariadicFunctionUsedVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::variadicFunctionUsed);
}

bool VariadicFunctionUsedVisitor::VisitFunctionDecl(
    const clang::FunctionDecl *FD) {
  if (FD->isVariadic()) {
    return !AutocheckDiagnostic::reportWarning(
                DE, FD->getBeginLoc(), AutocheckWarnings::variadicFunctionUsed)
                .limitReached();
  }
  return true;
}

/* Implementation of FunctionRedeclParamsVisitor */

FunctionRedeclParamsVisitor::FunctionRedeclParamsVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool FunctionRedeclParamsVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::functionRedeclParams);
}

bool FunctionRedeclParamsVisitor::VisitFunctionDecl(
    const clang::FunctionDecl *FD) {
  if (FD->isFirstDecl()) {
    const auto &Params = FD->parameters();
    const auto &ItEnd = FD->redecls_end();
    for (auto It = FD->redecls_begin(); It != ItEnd; ++It) {
      const auto &RedeclParams = It->parameters();
      unsigned NumOfParams = FD->getNumParams();
      for (unsigned i = 0; i < NumOfParams; ++i) {
        const llvm::StringRef &ParamName = Params[i]->getName();
        const llvm::StringRef &RedeclParamName = RedeclParams[i]->getName();
        if (!ParamName.empty() && !RedeclParamName.empty() &&
            (ParamName != RedeclParamName)) {
          if (AutocheckDiagnostic::reportWarning(
                  DE, It->getBeginLoc(),
                  AutocheckWarnings::functionRedeclParams)
                  .limitReached())
            return false;
        }
      }
    }
  }

  return true;
}

/* Implementation of AutoVarBracedInitVisitor */

AutoVarBracedInitVisitor::AutoVarBracedInitVisitor(clang::DiagnosticsEngine &DE,
                                                   clang::Sema &SemaRef)
    : DE(DE), SemaRef(SemaRef) {}

bool AutoVarBracedInitVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::autoVarBracedInit);
}

bool AutoVarBracedInitVisitor::VisitVarDecl(const clang::VarDecl *VD) {
  if (VD->hasInit()) {
    const clang::QualType &InitQualType = VD->getInit()->getType();
    if (VD->getTypeSourceInfo()
            ->getTypeLoc()
            .getUnqualifiedLoc()
            .getTypeLocClass() == clang::TypeLoc::Auto) {
      const clang::VarDecl::InitializationStyle &IS = VD->getInitStyle();
      // Call-style initialization (e.g. int x(0)).
      if (IS == clang::VarDecl::InitializationStyle::CallInit) {
        return true;
        // Direct list-initialization (e.g. int x {0}).
      } else if (IS == clang::VarDecl::InitializationStyle::ListInit) {
        return !AutocheckDiagnostic::reportWarning(
                    DE, VD->getBeginLoc(), AutocheckWarnings::autoVarBracedInit)
                    .limitReached();
        // C-style initialization with assignment (e.g. int x = {0}).
      } else {
        if (SemaRef.isStdInitializerList(InitQualType, nullptr)) {
          return !AutocheckDiagnostic::reportWarning(
                      DE, VD->getBeginLoc(),
                      AutocheckWarnings::autoVarBracedInit)
                      .limitReached();
        }
      }
    }
  }

  return true;
}

/* Implementation of VarBracedInitVisitor */

VarBracedInitVisitor::VarBracedInitVisitor(clang::DiagnosticsEngine &DE,
                                           clang::ASTContext &AC,
                                           clang::Sema &SemaRef)
    : DE(DE), AC(AC), SemaRef(SemaRef) {}

bool VarBracedInitVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::nonBracedInit);
}

bool inTemplateFunctionInstantiation(const clang::Decl *D) {
  const clang::DeclContext *Context = D->getDeclContext();
  while (Context) {
    if (const clang::FunctionDecl *ParentFunction =
            llvm::dyn_cast_if_present<clang::FunctionDecl>(Context)) {
      if (ParentFunction->isTemplateInstantiation())
        return true;
    }
    Context = Context->getParent();
  }
  return false;
}

bool VarBracedInitVisitor::VisitVarDecl(const clang::VarDecl *VD) {
  // Don't warn in template instantiation, we had already warned about this in
  // template declaration.
  if (inTemplateFunctionInstantiation(VD))
    return true;

  // Default arguments must be initialized with equals sign ('=').
  if (llvm::dyn_cast_if_present<const clang::ParmVarDecl>(VD))
    return true;
  // Don't warn if it is auto deduced type.
  if (VD->getType()->getContainedAutoType())
    return true;
  if (VD->hasInit()) {
    if (VD->getInitStyle() != clang::VarDecl::InitializationStyle::ListInit) {
      if (const clang::CXXRecordDecl *CRD =
              VD->getType()->getAsCXXRecordDecl()) {
        const auto &EndIt = CRD->ctor_end();
        for (auto It = CRD->ctor_begin(); It != EndIt; ++It) {
          if (SemaRef.isInitListConstructor(*It))
            return true;
        }
      }
      // Check if it's in the declaration of a range-based for loop.
      const auto &Parents = AC.getParents(*VD);
      assert((Parents.size() > 0) && "At least one parent node expected.");
      auto Parent = Parents.begin();
      if (const clang::DeclStmt *DS = Parent->get<clang::DeclStmt>()) {
        Parent = AC.getParents(*DS).begin();
        if (Parent->get<clang::CXXForRangeStmt>())
          return true;
      }
      if (!AC.getLangOpts().OpenCL) {
        return !AutocheckDiagnostic::reportWarning(
                    DE, VD->getLocation(), AutocheckWarnings::nonBracedInit)
                    .limitReached();
      }
    }
  }

  return true;
}

/* Implementation of CVQualifiersPlacedLeftVisitor */

CVQualifiersPlacedLeftVisitor::CVQualifiersPlacedLeftVisitor(
    clang::SourceManager &SM, clang::DiagnosticsEngine &DE,
    clang::Sema &SemaRef)
    : SM(SM), DE(DE), SemaRef(SemaRef) {}

bool CVQualifiersPlacedLeftVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::cvQualifiersPlacedLeft);
}

bool CVQualifiersPlacedLeftVisitor::VisitVarDecl(const clang::VarDecl *VD) {
  const auto &DeclLoc = VD->getBeginLoc();
  const auto &DeclEndLoc = VD->getEndLoc();

  if (!DeclLoc.isValid() || !DeclEndLoc.isValid())
    return true;
  if (DeclLoc.isMacroID() || DeclEndLoc.isMacroID())
    return true;

  const clang::QualType &QT = VD->getType();
  const clang::TypedefType *TDT = QT.getTypePtr()->getAs<clang::TypedefType>();

  if (!TDT)
    return true;

  if (const clang::TypedefNameDecl *TDND = TDT->getDecl())
    if (TDND->getBeginLoc().isValid() &&
        AutocheckContext::Get().CheckSystemHeaders &&
        SM.isInSystemHeader(TDND->getBeginLoc()))
      return true;

  if (QT.isLocalConstQualified() || QT.isLocalVolatileQualified()) {
    std::string VDData(
        SM.getCharacterData(DeclLoc),
        SM.getCharacterData(SemaRef.getLocForEndOfToken(DeclEndLoc)));
    unsigned ConstPos = VDData.find("const");
    unsigned VolatilePos = VDData.find("volatile");
    std::string VDTypeName(QT.getLocalUnqualifiedType().getAsString());
    auto Idx = VDTypeName.find_last_of("::");
    std::string VDTypeNameStripNamespace =
        Idx == std::string::npos ? VDTypeName : VDTypeName.substr(Idx + 1);
    unsigned TypeNamePos = VDData.find(VDTypeNameStripNamespace);

    if (ConstPos < TypeNamePos || VolatilePos < TypeNamePos) {
      return !AutocheckDiagnostic::reportWarning(
                  DE, DeclLoc, AutocheckWarnings::cvQualifiersPlacedLeft)
                  .limitReached();
    }
  }

  return true;
}

/* Implementation of AssignmentOperatorReturn */

AssignmentOperatorReturn::AssignmentOperatorReturn(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool AssignmentOperatorReturn::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::assignmentOperatorReturnType);
}

bool AssignmentOperatorReturn::isWrongDeclarationReturnType(
    const clang::CXXMethodDecl *CMD) const {
  const clang::QualType ThisObjectType = CMD->getThisObjectType();
  const clang::QualType ReturnType = CMD->getDeclaredReturnType();

  // Return type can't have qualifiers and has to be of reference type.
  if (ReturnType.hasQualifiers() ||
      !ReturnType.getTypePtr()->isReferenceType()) {
    return true;
  }

  const clang::QualType NonReferencedReturnType =
      ReturnType.getNonReferenceType().getCanonicalType();

  // Return type without reference has to be the same as the type of "this"
  // object.
  if (NonReferencedReturnType != ThisObjectType) {
    return true;
  }

  return false;
}

bool AssignmentOperatorReturn::isWrongReturnStmt(
    const clang::CXXMethodDecl *CMD) const {
  clang::Stmt *MethodDefinition = CMD->getDefinition()->getBody();

  if (!MethodDefinition)
    return false;

  // Look for the return statement inside method definition and return true if
  // it's not in the following form: return *this;
  for (auto ChildNode = MethodDefinition->child_begin();
       ChildNode != MethodDefinition->child_end(); ChildNode++) {
    clang::ReturnStmt *RS =
        llvm::dyn_cast_if_present<clang::ReturnStmt>(*ChildNode);

    if (!RS)
      continue;

    // Ignore parentheses around UnaryOperator.
    clang::Expr *ParensExpr =
        llvm::dyn_cast_if_present<clang::Expr>(*(RS->child_begin()));
    clang::UnaryOperator *UnaryOperatorVariable =
        llvm::dyn_cast_if_present<clang::UnaryOperator>(
            ParensExpr->IgnoreParens());

    if (!UnaryOperatorVariable ||
        UnaryOperatorVariable->getOpcode() != clang::UO_Deref) {
      return true;
    }

    // Ignore parentheses around CXXThisExpr.
    ParensExpr = llvm::dyn_cast_if_present<clang::Expr>(
        *(UnaryOperatorVariable->child_begin()));
    clang::CXXThisExpr *CXXThisExprVariable =
        llvm::dyn_cast_if_present<clang::CXXThisExpr>(
            ParensExpr->IgnoreParens());

    if (!CXXThisExprVariable) {
      return true;
    }
  }
  return false;
}

bool AssignmentOperatorReturn::VisitCXXMethodDecl(
    const clang::CXXMethodDecl *CMD) {
  if (!CMD->isCopyAssignmentOperator() || !CMD->isThisDeclarationADefinition())
    return true;

  if (isWrongDeclarationReturnType(CMD) || isWrongReturnStmt(CMD)) {
    return !AutocheckDiagnostic::reportWarning(
                DE, CMD->getBeginLoc(),
                AutocheckWarnings::assignmentOperatorReturnType)
                .limitReached();
  }

  return true;
}

/* Implementation of ConstUnusedForImmutableData */

// This rule is applied to varables of the following types:
// - Primitive data types (integer, character, boolean, floating point, double
// floating point, wide character);
// - Derived data types (array, pointer, reference);
// - User-defined data types (class, structure, union, enumeration and typedef
// defined data types).
// The rule is not applied to C++ types void and function, because that doesn't
// make sense in this context.

ConstUnusedForImmutableData::ConstUnusedForImmutableData(
    clang::DiagnosticsEngine &DE, clang::ASTContext &AC)
    : DE(DE), AC(AC) {}

bool ConstUnusedForImmutableData::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::constUnusedForImmutableData);
}

void ConstUnusedForImmutableData::PostWork() {
  // Any variable not removed by ConstUnusedForImmutableData is immutable.
  for (auto &i : ImmutableVarCandidates) {
    if (AutocheckDiagnostic::reportWarning(
            DE, i, AutocheckWarnings::constUnusedForImmutableData)
            .limitReached())
      return;
  }
}

bool ConstUnusedForImmutableData::VisitVarDecl(const clang::VarDecl *VD) {
  // Collect all variables that are not declared as const or constexpr.
  // The exceptions are function parameters (according to Autosar document).
  if (llvm::dyn_cast_if_present<const clang::ParmVarDecl>(VD))
    return true;
  const clang::SourceLocation VarLoc = VD->getLocation();
  const clang::QualType &Qt = VD->getType();
  // In the case of reference type, we are looking for referenced variable.
  if (Qt->isReferenceType()) {
    if (!Qt.getNonReferenceType().isConstQualified())
      ImmutableVarCandidates.insert(VarLoc);
  } else { // Not a reference type
    // Ignore auto type variables.
    if ((!Qt.isNull() && !llvm::dyn_cast<clang::AutoType>(Qt) &&
         !Qt.isConstQualified())) {
      ImmutableVarCandidates.insert(VarLoc);
    }
  }
  return true;
}

bool ConstUnusedForImmutableData::IfLValueEraseFromImmCandidates(
    const clang::DeclRefExpr *DRE) {
  if (DRE->isLValue()) {
    const clang::SourceLocation VarLoc = DRE->getDecl()->getLocation();
    ImmutableVarCandidates.erase(VarLoc);
    return true;
  }
  return false;
}

const clang::DeclRefExpr *
ConstUnusedForImmutableData::FindDREInArraySubscript(const clang::Expr *E) {
  if (!E)
    return nullptr;
  const clang::ArraySubscriptExpr *ASE =
      llvm::dyn_cast_if_present<clang::ArraySubscriptExpr>(E->IgnoreImpCasts());
  if (!ASE)
    return nullptr;
  if (const clang::ImplicitCastExpr *ICE =
          llvm::dyn_cast_if_present<clang::ImplicitCastExpr>(ASE->getBase())) {
    if (const clang::DeclRefExpr *DRE =
            llvm::dyn_cast_if_present<clang::DeclRefExpr>(ICE->getSubExpr()))
      return DRE;
    else
      return FindDREInArraySubscript(ICE->getSubExpr());
  } else {
    if (const clang::DeclRefExpr *DRE =
            llvm::dyn_cast_if_present<clang::DeclRefExpr>(ASE->getBase()))
      return DRE;
    else
      return FindDREInArraySubscript(ASE->getBase());
  }
}

bool ConstUnusedForImmutableData::CheckIfOperandIsArraySubscript(
    const clang::Expr *E) {
  // Check if the operand is array element and in case it is, delete array from
  // immutable var candidates set.
  if (const clang::DeclRefExpr *DRE =
          FindDREInArraySubscript(E->IgnoreImpCasts()))
    return IfLValueEraseFromImmCandidates(DRE);
  return false;
}

bool ConstUnusedForImmutableData::CheckIfOperandIsUserDefinedType(
    const clang::Expr *E) {
  // Check if the operand is user defined data type, and in the case it is,
  // delete it from immutable var candidates set. Works for maximum nested
  // level 2.
  if (const clang::MemberExpr *ME =
          llvm::dyn_cast_if_present<clang::MemberExpr>(E)) {
    const clang::DeclRefExpr *DRE = nullptr;
    if (const clang::MemberExpr *MME =
            llvm::dyn_cast_if_present<clang::MemberExpr>(ME->getBase()))
      DRE = llvm::dyn_cast_if_present<clang::DeclRefExpr>(MME->getBase());
    else
      DRE = llvm::dyn_cast_if_present<clang::DeclRefExpr>(ME->getBase());
    if (DRE)
      return IfLValueEraseFromImmCandidates(DRE);
  }
  return false;
}

bool ConstUnusedForImmutableData::CheckIfOperandIsOperatorSubscript(
    const clang::Expr *E) {
  // Check if the operand is array class object to which the operator subscript
  // is applied.
  if (const clang::CXXOperatorCallExpr *CXXOCE =
          llvm::dyn_cast<clang::CXXOperatorCallExpr>(E)) {
    clang::OverloadedOperatorKind Op = CXXOCE->getOperator();
    if (Op == clang::OO_Subscript) {
      if (const clang::DeclRefExpr *DRE =
              llvm::dyn_cast_if_present<const clang::DeclRefExpr>(
                  CXXOCE->getArg(0)->IgnoreParenCasts())) {
        if (const clang::VarDecl *VD =
                llvm::dyn_cast_if_present<const clang::VarDecl>(
                    DRE->getDecl())) {
          ImmutableVarCandidates.erase(VD->getLocation());
        }
      }
    }
  }
  return true;
}

bool ConstUnusedForImmutableData::CheckIfOperandIsPrimitiveOrPtrOrRefType(
    const clang::Expr *E) {
  // If the operand is primitive type variable, delete it from the immutable var
  // candidates set.
  if (const clang::DeclRefExpr *DRE =
          llvm::dyn_cast_if_present<clang::DeclRefExpr>(E))
    return IfLValueEraseFromImmCandidates(DRE);
  return false;
}

bool ConstUnusedForImmutableData::VisitUnaryOperator(
    const clang::UnaryOperator *UO) {
  const clang::Expr *SubExpr = UO->getSubExpr();
  if (!SubExpr)
    return true;
  // Check operand.
  if (CheckIfOperandIsArraySubscript(SubExpr) ||
      CheckIfOperandIsUserDefinedType(SubExpr) ||
      CheckIfOperandIsPrimitiveOrPtrOrRefType(SubExpr) ||
      CheckIfOperandIsOperatorSubscript(SubExpr))
    return true;
  return true;
}

bool ConstUnusedForImmutableData::VisitBinaryOperator(
    const clang::BinaryOperator *BO) {
  const clang::Expr *LHS = BO->getLHS();
  const clang::Expr *RHS = BO->getRHS();
  if (!LHS || !RHS)
    return true;
  // Check operand on the left side of binary operator.
  if (CheckIfOperandIsArraySubscript(LHS) ||
      CheckIfOperandIsUserDefinedType(LHS) ||
      CheckIfOperandIsPrimitiveOrPtrOrRefType(LHS) ||
      CheckIfOperandIsOperatorSubscript(LHS))
    return true;
  return true;
}

bool ConstUnusedForImmutableData::CheckFunctionArguments(
    const clang::CallExpr *CE) {
  // Check if function parameters are not declared as const pointer or
  // reference type and delete appropriate arguments from immutable var
  // candidates set.
  unsigned NumOfArgs = CE->getNumArgs();
  if (NumOfArgs == 0)
    return true;
  const clang::FunctionDecl *FD = CE->getDirectCallee();
  if (!FD)
    return true;
  unsigned NumOfParams = FD->getNumParams();
  const clang::DeclRefExpr *DRE;
  const clang::VarDecl *VD;
  for (unsigned i = 0; i < NumOfParams; ++i) {
    clang::QualType Qt = FD->parameters()[i]->getType();
    bool isConstQualified = false;
    if (Qt->isPointerType()) {
      isConstQualified = Qt->getPointeeType().isConstQualified();
    } else if (Qt->isReferenceType())
      isConstQualified = Qt.getNonReferenceType().isConstQualified();
    if ((Qt->isPointerType() || Qt->isReferenceType()) && !isConstQualified &&
        i < NumOfArgs) {
      unsigned argNum = NumOfArgs > NumOfParams ? i + 1 : i;
      const clang::Expr *ArgExpr = (CE->getArg(argNum))->IgnoreParenCasts();
      DRE = llvm::dyn_cast_if_present<const clang::DeclRefExpr>(ArgExpr);
      if (!DRE)
        return true;
      VD = llvm::dyn_cast_if_present<const clang::VarDecl>(DRE->getDecl());
      if (!VD)
        return true;
      // If the argument is array or variable associated with the reference,
      // erase it from the immutable candidates. If the argument is pointer or
      // reference that is passed by value, do not erase it.
      if (!areTypesEquivalent(AC, Qt.getUnqualifiedType(), VD->getType()))
        ImmutableVarCandidates.erase(VD->getLocation());
    }
  }
  return true;
}

bool ConstUnusedForImmutableData::CheckOperatorCallExpr(
    const clang::CallExpr *CE) {
  if (const clang::CXXOperatorCallExpr *CXXOCE =
          llvm::dyn_cast<clang::CXXOperatorCallExpr>(CE)) {
    const clang::CXXMethodDecl *MD =
        llvm::dyn_cast_if_present<const clang::CXXMethodDecl>(
            CXXOCE->getDirectCallee());
    if (MD && !MD->isConst()) {
      // Trust that an std operator[] doesn't change the calling object.
      if (CXXOCE->getNumArgs() != 0) {
        if (CXXOCE->getOperator() == clang::OO_Subscript &&
            CXXOCE->getArg(0)->getType().getAsString().rfind("std::", 0) == 0) {
          // Check that the returned reference is either cast into an RValue or
          // assigned to a const type.
          const auto &Parents = AC.getParents(*CXXOCE);
          const unsigned NumberOfParents = Parents.size();
          assert((NumberOfParents > 0) && "At least one parent node expected.");
          if (const clang::ImplicitCastExpr *SE =
                  Parents[NumberOfParents - 1].get<clang::ImplicitCastExpr>()) {
            const clang::CastKind castKind = SE->getCastKind();
            if (castKind == clang::CastKind::CK_LValueToRValue ||
                castKind == clang::CastKind::CK_NoOp)
              return true;
          }
        }
      }
      if (const clang::DeclRefExpr *DRE =
              llvm::dyn_cast_if_present<const clang::DeclRefExpr>(
                  CXXOCE->getArg(0)->IgnoreParenCasts())) {
        if (const clang::VarDecl *VD =
                llvm::dyn_cast_if_present<const clang::VarDecl>(DRE->getDecl()))
          ImmutableVarCandidates.erase(VD->getLocation());
      }
    }
  }
  return true;
}

bool ConstUnusedForImmutableData::CheckMemberCallExpr(
    const clang::CallExpr *CE) {
  if (const clang::CXXMemberCallExpr *CXXMCE =
          llvm::dyn_cast<clang::CXXMemberCallExpr>(CE)) {
    const clang::MemberExpr *ME =
        llvm::dyn_cast<clang::MemberExpr>(CXXMCE->getCallee());
    const clang::CXXMethodDecl *MD = CXXMCE->getMethodDecl();
    if (ME && MD && !MD->isConst()) {
      if (const clang::DeclRefExpr *DRE =
              llvm::dyn_cast_if_present<const clang::DeclRefExpr>(
                  ME->getBase()))
        if (const clang::VarDecl *VD =
                llvm::dyn_cast_if_present<const clang::VarDecl>(DRE->getDecl()))
          ImmutableVarCandidates.erase(VD->getLocation());
    }
    return true;
  }
  return true;
}

bool ConstUnusedForImmutableData::VisitCallExpr(const clang::CallExpr *CE) {
  if (const clang::CXXMemberCallExpr *CXXMCE =
          llvm::dyn_cast<clang::CXXMemberCallExpr>(CE))
    CheckMemberCallExpr(CXXMCE);
  if (const clang::CXXOperatorCallExpr *CXXOCE =
          llvm::dyn_cast<clang::CXXOperatorCallExpr>(CE))
    CheckOperatorCallExpr(CXXOCE);
  return CheckFunctionArguments(CE);
}

/* Implementation of NSDMIAndConsutructorInitUsed */

NSDMIAndConsutructorInitUsed::NSDMIAndConsutructorInitUsed(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool NSDMIAndConsutructorInitUsed::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::nsdmiAndCtorInitUsed);
}

bool NSDMIAndConsutructorInitUsed::VisitCXXRecordDecl(
    const clang::CXXRecordDecl *CRD) {
  if (CRD->isImplicit() || !CRD->isThisDeclarationADefinition())
    return true;

  for (clang::CXXConstructorDecl *ctor : CRD->ctors()) {
    if (CRD->isImplicit() || !CRD->isThisDeclarationADefinition())
      return true;
    // Move and copy constructors are exception to the rule (see Autosar pdf).
    if (!ctor->isCopyConstructor() && !ctor->isMoveConstructor()) {
      for (clang::CXXCtorInitializer *init : ctor->inits()) {
        if (init->isMemberInitializer()) {
          if (init->getMember()->getInClassInitializer()) {
            bool stopVisitor = !AutocheckDiagnostic::reportWarning(
                                    DE, init->getInit()->getBeginLoc(),
                                    AutocheckWarnings::nsdmiAndCtorInitUsed)
                                    .limitReached();
            AutocheckDiagnostic::reportWarning(
                DE, init->getMember()->getBeginLoc(),
                AutocheckWarnings::notePreviousMemberInitialization);
            return stopVisitor;
          }
        }
      }
    }
  }
  return true;
}

/* Implementation of RandomEngineDefaultInitialized */

RandomEngineDefaultInitialized::RandomEngineDefaultInitialized(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool RandomEngineDefaultInitialized::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::randomEngineDefaultInitialized);
}

bool RandomEngineDefaultInitialized::VisitCXXConstructExpr(
    const clang::CXXConstructExpr *CCE) {
  if (isAcceptableEngine(CCE->getConstructor()->getDeclName().getAsString())) {
    clang::SourceLocation noteLoc;
    if (CCE->getNumArgs() > 0) {
      const clang::Expr *Arg = CCE->getArg(0)->IgnoreParenImpCasts();
      noteLoc = Arg->getBeginLoc();
      // Default initialization.
      if (Arg->isDefaultArgument()) {
        return !AutocheckDiagnostic::reportWarning(
                    DE, CCE->getBeginLoc(),
                    AutocheckWarnings::randomEngineDefaultInitialized)
                    .limitReached();
      }
      if (const clang::CallExpr *CE =
              llvm::dyn_cast_if_present<clang::CallExpr>(Arg)) {
        if (const clang::CXXMethodDecl *CMD =
                llvm::dyn_cast_if_present<clang::CXXMethodDecl>(
                    CE->getDirectCallee())) {
          const clang::CXXRecordDecl *Parent = CMD->getParent();
          // Based on SEI CERT C++ Coding Standard MSC51-CPP:
          // Allow engine initialization with random_device from <random>
          // header.
          if (Parent->getNameAsString() == randomDeviceName &&
              DE.getSourceManager().isInSystemHeader(Parent->getLocation())) {
            return true;
          }
        }
      } else if (const clang::DeclRefExpr *DRE =
                     llvm::dyn_cast_if_present<clang::DeclRefExpr>(Arg)) {
        if (const clang::VarDecl *VD =
                llvm::dyn_cast_if_present<clang::VarDecl>(DRE->getDecl())) {
          // Allow values that are produced via random_device's () operator
          if (const clang::CXXOperatorCallExpr *COCE =
                  llvm::dyn_cast_if_present<clang::CXXOperatorCallExpr>(
                      VD->getDefinition()->getInit())) {
            if ((*COCE->arg_begin())
                    ->getType()
                    .getBaseTypeIdentifier()
                    ->getName() == randomDeviceName) {
              return true;
            }
          }
        }
      }
    }
    // No argument constructor call is not allowed because seed will be default
    // and unsecure.
    bool stopVisitor = AutocheckDiagnostic::reportWarning(
                           DE, CCE->getBeginLoc(),
                           AutocheckWarnings::randomEngineDefaultInitialized)
                           .limitReached();
    // Report note if there was at least one constructor argument.
    if (noteLoc.isValid())
      AutocheckDiagnostic::reportWarning(DE, noteLoc,
                                         AutocheckWarnings::noteUnsafeSeed);
    if (stopVisitor)
      return false;
  }
  return true;
}

bool RandomEngineDefaultInitialized::isAcceptableEngine(
    const std::string &Name) const {
  if (std::find(engines.cbegin(), engines.cend(), Name) != engines.cend()) {
    return true;
  }
  return false;
}

/* Implementation of BroadScopeIdentifierVisitor */

BroadScopeIdentifierVisitor::BroadScopeIdentifierVisitor(
    clang::DiagnosticsEngine &DE, clang::ASTContext &ASTCtx)
    : DE(DE), ASTCtx(ASTCtx) {}

bool BroadScopeIdentifierVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::broadScopeIdentifier);
}

bool BroadScopeIdentifierVisitor::VisitVarDecl(const clang::VarDecl *VD) {
  // Ignore function parameter declarations, static data members, static
  // local variables and duplicated variable template specializations. If
  // variable is declared inside of If statement's condition part, it is okay to
  // use it throughout If statement, even in nested scopes.
  if (!llvm::dyn_cast_if_present<clang::ParmVarDecl>(VD) &&
      !VD->isStaticDataMember() && !VD->isStaticLocal() && !isIfCond &&
      !llvm::dyn_cast_if_present<clang::VarTemplateSpecializationDecl>(VD)) {
    // If the variable is ever referenced, there is a chance it's usage is
    // illegal.
    if (VD->isReferenced()) {
      PotentiallyBadVars.insert(std::make_pair(VD, VarInfo(StmtScopes.size())));
    }
    if (isForInit) {
      // Save type of variable declared in the init part of for loop.
      forInitType = VD->getType();
    } else {
      // Reset init type.
      forInitType = clang::QualType();
    }
  }
  return true;
}

bool BroadScopeIdentifierVisitor::UpdateStatesOnEndOfScope() {
  unsigned short Level = StmtScopes.size();

  for (auto It = PotentiallyBadVars.begin(); It != PotentiallyBadVars.end();) {
    const VarState &State = getVarState(It);
    unsigned short DeclLevel = getDeclLevel(It);

    if (DeclLevel < Level) {
      ++It;
    } else if (DeclLevel == Level) {
      if (State == VarState::ReferencedDeeper)
        setVarState(It, VarState::ReferencedDeeperAndResurfaced);
      ++It;
    } else if (DeclLevel > Level) {
      // Variable is used in deeper scope than necessary, it could have been
      // lowered.
      if (State == VarState::ReferencedDeeper ||
          State == VarState::ReferencedDeeperAndResurfaced) {
        AutocheckDiagnostic::reportWarning(
            DE, It->first->getLocation(),
            AutocheckWarnings::broadScopeIdentifier);
      }
      It = PotentiallyBadVars.erase(It);
    }
  }

  return true;
}

bool BroadScopeIdentifierVisitor::VisitDeclRefExpr(
    const clang::DeclRefExpr *DRE) {
  auto It = PotentiallyBadVars.find(DRE->getDecl());
  if (It != PotentiallyBadVars.end()) {
    unsigned short DeclLevel = getDeclLevel(It);
    const VarState &State = getVarState(It);
    if (isInFor) {
      // Variable cannot be lowered into the Init part of the for loop if it has
      // different type to the one previously declared here or if it's used in
      // any expression other than some form of assignment.
      const auto &Parents = ASTCtx.getParents(*DRE);
      if ((!forInitType.isNull() &&
           !areTypesEquivalent(ASTCtx, forInitType, It->first->getType())) ||
          (isForInit && !(Parents[0].get<clang::CompoundAssignOperator>()) &&
           !(Parents[0].get<clang::BinaryOperator>()))) {
        PotentiallyBadVars.erase(It);
        return true;
      }
    }
    if (DeclLevel < StmtScopes.size()) {
      // Update the variable state.
      if (State == VarState::NotYetReferenced) {
        setVarState(It, VarState::ReferencedDeeper);
      }
      // Variable is used on multiple lower scopes and therefore it cannot be
      // lowered.
      else if (State == VarState::ReferencedDeeperAndResurfaced) {
        PotentiallyBadVars.erase(It);
      }
    } // The variable is used within the same scope in which it is declared and
    // it cannot be lowered.
    else if (DeclLevel == StmtScopes.size()) {
      PotentiallyBadVars.erase(It);
    } // It is not possible to reference variable that is not yet declared.
    else if (DeclLevel > StmtScopes.size()) {
      llvm_unreachable("Reference out of scope\n");
    }
  }
  return true;
}

bool BroadScopeIdentifierVisitor::PreTraverseStmt(clang::Stmt *S) {
  if (isNewScope(S))
    StmtScopes.push_back(S);

  return true;
}

bool BroadScopeIdentifierVisitor::PostTraverseStmt(clang::Stmt *S) {
  if (!StmtScopes.empty() && StmtScopes.back() == S) {
    StmtScopes.pop_back();
    isForInit = false;
    isIfCond = false;
    UpdateStatesOnEndOfScope();
  }
  if (llvm::dyn_cast_if_present<clang::ForStmt>(S) ||
      llvm::dyn_cast_if_present<clang::CXXForRangeStmt>(S)) {
    isInFor = false;
  }
  return true;
}

void BroadScopeIdentifierVisitor::PostWork() {
  // Handle global variables at the end of translation unit declaration:
  // all variables still present in a map are bad and could have been lowered in
  // a more shallow scope.
  for (auto v : PotentiallyBadVars) {
    if (AutocheckDiagnostic::reportWarning(
            DE, v.first->getLocation(), AutocheckWarnings::broadScopeIdentifier)
            .limitReached())
      break;
  }
}

// Check wheter this statement starts a new scope:
// Init part of for loop and Cond part of if statement should be traversed
// additionally.
bool BroadScopeIdentifierVisitor::isNewScope(const clang::Stmt *S) {
  bool isScope = false;

  if (!StmtsForTraversal.empty() && StmtsForTraversal.back() == S) {
    StmtsForTraversal.pop_back();
    isScope = true;
  }

  if (const clang::ForStmt *FS = llvm::dyn_cast_if_present<clang::ForStmt>(S)) {
    isInFor = true;
    isForInit = true;
    StmtsForTraversal.push_back(FS->getInit());
    isScope = true;
  } else if (const clang::CompoundStmt *CS =
                 llvm::dyn_cast_if_present<clang::CompoundStmt>(S)) {
    // Compound statement that has ForStmt/CXXForRangeStmt as a parent should
    // not start a new scope because it is already in
    // ForStmt's/CXXForRangeStmt's scope. Ignore while and do while statements.
    const auto &Parents = ASTCtx.getParents(*CS);
    if (!Parents[0].get<clang::ForStmt>() && !Parents[0].get<clang::DoStmt>() &&
        !Parents[0].get<clang::WhileStmt>() &&
        !Parents[0].get<clang::CXXForRangeStmt>() &&
        !Parents[0].get<clang::IfStmt>())
      isScope = true;
  } else if (const clang::IfStmt *IS =
                 llvm::dyn_cast_if_present<clang::IfStmt>(S)) {
    isIfCond = true;
    if (const clang::Stmt *ElseStmt = IS->getElse()) {
      if (!llvm::dyn_cast_if_present<clang::IfStmt>(ElseStmt))
        StmtsForTraversal.push_back(ElseStmt);
    }
    StmtsForTraversal.push_back(IS->getThen());
  } else if (const clang::CXXForRangeStmt *CFRS =
                 llvm::dyn_cast_if_present<clang::CXXForRangeStmt>(S)) {
    // (until C++20) Range based for loop doesn't allow any statement other than
    // range_declaration.
    isInFor = true;
    isForInit = true;
    isScope = true;
    StmtsForTraversal.push_back(CFRS->getRangeInit());
  } else if (llvm::dyn_cast_if_present<clang::CaseStmt>(S)) {
    isScope = true;
  }
  return isScope;
}

/* Implementation of MissingBracesOrEltsVisitor */

MissingBracesOrEltsVisitor::MissingBracesOrEltsVisitor(
    clang::DiagnosticsEngine &DE, clang::Sema &SemaRef)
    : DE(DE), SM(DE.getSourceManager()), SemaRef(SemaRef) {}

bool MissingBracesOrEltsVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::missingBracesOrElements);
}

bool MissingBracesOrEltsVisitor::hasBraces(const clang::InitListExpr *ILE) {
  return *SM.getCharacterData(ILE->getLBraceLoc()) == '{';
}

bool MissingBracesOrEltsVisitor::hasExactlyOneElt(
    const clang::InitListExpr *ILE) {
  // ILE need to have exactly one child ...
  if (ILE->getNumInits() != 1)
    return false;

  //... and one array filler ...
  if (!ILE->hasArrayFiller())
    return false;

  //... when that one child is InitListExpr (without braces) recourse.
  const clang::InitListExpr *ILEchild =
      llvm::dyn_cast_if_present<clang::InitListExpr>(*ILE->child_begin());
  if (ILEchild) {
    if (!hasBraces(ILEchild))
      return hasExactlyOneElt(ILEchild);
    return false;
  }

  //... otherwise child must be 'zero' literal for its type.
  const clang::Stmt *ExpectingLiteral = *ILE->child_begin();
  const clang::ImplicitCastExpr *ImplCast =
      llvm::dyn_cast_if_present<clang::ImplicitCastExpr>(ExpectingLiteral);

  // Check whether Src is nullptr, Dest is already a pointer type.
  if (ImplCast &&
      ImplCast->getCastKind() == clang::CastKind::CK_NullToPointer) {
    const clang::CXXNullPtrLiteralExpr *NullPtr =
        llvm::dyn_cast_if_present<clang::CXXNullPtrLiteralExpr>(
            ImplCast->IgnoreImpCasts());
    return NullPtr ? true : false;
  }

  // Ignore clang::ImplicitCastExpr when type does not have dedicated literal
  // format i.e. allow use of: '0' for 'int8_t' and 'int16_t', '0u' for
  // 'uint8_t' and 'uint16_t'.
  if (ImplCast && ImplCast->getCastKind() == clang::CastKind::CK_IntegralCast) {
    const clang::QualType &DestType = ImplCast->getType();
    const clang::QualType &SrcType = ImplCast->IgnoreImpCasts()->getType();
    if (DestType->isBuiltinType() && SrcType->isBuiltinType()) {
      unsigned DestKind = DestType->getAs<clang::BuiltinType>()->getKind();
      unsigned SrcKind = SrcType->getAs<clang::BuiltinType>()->getKind();
      // int8_t is Char_S, char is SChar.
      if ((SrcKind == clang::BuiltinType::Int &&
           (DestKind == clang::BuiltinType::Short ||
            DestKind == clang::BuiltinType::Char_S ||
            DestKind == clang::BuiltinType::SChar)) ||
          (SrcKind == clang::BuiltinType::UInt &&
           (DestKind == clang::BuiltinType::UShort ||
            DestKind == clang::BuiltinType::UChar)))
        ExpectingLiteral = ImplCast->IgnoreImpCasts();
      else
        return false;
    }
  }

  const clang::IntegerLiteral *Int =
      llvm::dyn_cast_if_present<clang::IntegerLiteral>(ExpectingLiteral);
  if (Int && Int->getValue().isNullValue())
    return true;

  const clang::FloatingLiteral *FP =
      llvm::dyn_cast_if_present<clang::FloatingLiteral>(ExpectingLiteral);
  if (FP && FP->getValue().isZero())
    return true;

  const clang::CXXBoolLiteralExpr *Bool =
      llvm::dyn_cast_if_present<clang::CXXBoolLiteralExpr>(ExpectingLiteral);
  if (Bool && Bool->getValue() == false)
    return true;

  return false;
}

bool MissingBracesOrEltsVisitor::isSubExprEmpty(
    const clang::Stmt *SubStatement) {
  if (const clang::ImplicitValueInitExpr *ImplValue =
          llvm::dyn_cast_if_present<clang::ImplicitValueInitExpr>(
              SubStatement)) {
    assert((ImplValue->child_begin() == ImplValue->child_end()) &&
           "FixMe: Unexpectedly clang::ImplicitValueInitExpr has children");
    // clang::ImplicitValueInitExpr is fine, don't check its contents.
    return true;
  } else if (const clang::InitListExpr *InitList =
                 llvm::dyn_cast_if_present<clang::InitListExpr>(SubStatement)) {
    return !hasBraces(InitList) && isEmpty(InitList);
  }
  return false;
}

bool MissingBracesOrEltsVisitor::isEmpty(const clang::InitListExpr *ILE) {
  if (ILE->hasArrayFiller() && !isSubExprEmpty(ILE->getArrayFiller()))
    return false;

  for (auto Child : ILE->children())
    if (!isSubExprEmpty(Child))
      return false;

  return true;
}

bool MissingBracesOrEltsVisitor::hasMissingEltOrBraces(
    const clang::InitListExpr *ILE, InitListInfo &ILI) {
  for (auto Child : ILE->children()) {
    const clang::InitListExpr *ILEchild =
        llvm::dyn_cast_if_present<clang::InitListExpr>(Child);
    if (ILEchild) {
      if (hasMissingEltOrBraces(ILEchild, ILI))
        return true;
    }

    const clang::ImplicitValueInitExpr *ImplValue =
        llvm::dyn_cast_if_present<clang::ImplicitValueInitExpr>(Child);
    if (ImplValue) {
      ILI.MissingElt = true;
      ILI.FirstImplValInit = ImplValue;
      ILI.BadILE = ILE;
      return true;
    }
  }

  if (ILE->hasArrayFiller()) {
    for (auto Child = ILE->child_begin(); Child != ILE->child_end(); ++Child)
      ILI.NumOfElts++;
    ILI.MissingElt = true;
    ILI.BadILE = ILE;
    return true;
  }

  // No elements are missing. Check if braces are missing.
  if (!hasBraces(ILE)) {
    ILI.BadILE = ILE;
    return true;
  }

  return false;
}

bool MissingBracesOrEltsVisitor::reportMissingEltOrBraces(
    InitListInfo &ILI, std::stringstream &EltHint) {
  // BadILEEnd points to beginning of last token, find its end and print green
  // letters at intended place. This is done automatically for SourceRange but
  // for SourceLocaions we have to do it ourselves.
  auto BadILEBegin = ILI.BadILE->getSourceRange().getBegin();
  auto BadILEEnd = ILI.BadILE->getSourceRange().getEnd();
  auto BadILETokenEnd = hasBraces(ILI.BadILE)
                            ? BadILEEnd
                            : SemaRef.getLocForEndOfToken(BadILEEnd);
  auto HelpfulHintLoc = BadILETokenEnd.getLocWithOffset(2);

  return AutocheckDiagnostic::reportWarning(
             DE, ILI.MissingElt ? BadILETokenEnd : BadILEBegin,
             AutocheckWarnings::missingBracesOrElements,
             ILI.BadILE->getSourceRange(),
             clang::FixItHint::CreateInsertion(BadILEBegin, "{"),
             clang::FixItHint::CreateInsertion(BadILETokenEnd, "}"),
             clang::FixItHint::CreateInsertion(HelpfulHintLoc, EltHint.str()))
      .limitReached();
}

bool MissingBracesOrEltsVisitor::VisitVarDecl(const clang::VarDecl *VD) {
  const clang::Expr *Init = VD->getInit();
  clang::QualType QT = VD->getType();

  const clang::InitListExpr *ILE =
      llvm::dyn_cast_if_present<clang::InitListExpr>(Init);
  if (!ILE)
    return true;

  // Single element zero initialization, only for arrays (multidimensional
  // arrays included) of default types (e.g. short, double).
  if (QT->isArrayType()) {
    const clang::Type *EltType = QT->getArrayElementTypeNoTypeQual();
    while (EltType->isArrayType())
      EltType = EltType->getArrayElementTypeNoTypeQual();
    if ((EltType->isBuiltinType() || EltType->isPointerType()) &&
        hasExactlyOneElt(ILE)) {
      return true;
    }
  }

  // Zero initialization using '{}'.
  if (isEmpty(ILE))
    return true;

  InitListInfo ILI{};
  if (hasMissingEltOrBraces(ILE, ILI)) {
    std::stringstream EltHint;

    // Missing element in structure like initialization list.
    if (ILI.MissingElt && ILI.FirstImplValInit) {
      EltHint << "Missing element of \'"
              << ILI.FirstImplValInit->getType().getAsString()
              << "\' type from initialization list for \'"
              << ILI.BadILE->getType().getAsString() << "\'";
      return !reportMissingEltOrBraces(ILI, EltHint);
    }

    // Missing element in array initialization list.
    if (ILI.MissingElt && !ILI.FirstImplValInit) {
      const clang::ArrayType *AType =
          ILI.BadILE->getType()->getAsArrayTypeUnsafe();
      const clang::ConstantArrayType *CATy =
          llvm::dyn_cast_if_present<clang::ConstantArrayType>(AType);
      unsigned NumOfMissingElts =
          CATy->getSize().getLimitedValue() - ILI.NumOfElts;
      EltHint << "Missing " << NumOfMissingElts << " array element"
              << (NumOfMissingElts > 1 ? "s" : "") << " of \'"
              << CATy->getElementType().getAsString() << "\' type from \'"
              << ILI.BadILE->getType().getAsString() << "\'";
      return !reportMissingEltOrBraces(ILI, EltHint);
    }

    // Braces are missing, same hint is written for both arrays and structs.
    EltHint << "Missing surrounding braces for \'"
            << ILI.BadILE->getType().getAsString() << "\'";
    return !reportMissingEltOrBraces(ILI, EltHint);
  }
  return true;
}

/* Implementation of ConstantInitializer */

ConstantInitializer::ConstantInitializer(clang::DiagnosticsEngine &DE,
                                         clang::ASTContext &AC,
                                         clang::Sema &SemaRef)
    : DE(DE), AC(AC), SemaRef(SemaRef) {}

bool ConstantInitializer::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::constantInitializer);
}

bool ConstantInitializer::VisitVarDecl(const clang::VarDecl *VD) {
  // Skip template specializations. Only check the VarDecl in the variable
  // template declaration.
  if (llvm::dyn_cast_if_present<clang::VarTemplateSpecializationDecl>(VD))
    return true;

  clang::StorageDuration SD = VD->getStorageDuration();
  if (SD == clang::StorageDuration::SD_Static ||
      SD == clang::StorageDuration::SD_Thread) {
    const clang::Expr *Init = VD->getInit();
    // Check if VD is uninitialized.
    if (!Init && !hasRedeclaration(VD)) {
      return !AutocheckDiagnostic::reportWarning(
                  DE, SemaRef.getLocForEndOfToken(VD->getEndLoc()),
                  AutocheckWarnings::constantInitializer, VD->getSourceRange())
                  .limitReached();
    }
    // Check if VD is initialized, but not constant initialized.
    if (Init && !Init->isValueDependent() &&
        !Init->isConstantInitializer(AC, false)) {
      return !AutocheckDiagnostic::reportWarning(
                  DE, Init->getBeginLoc(),
                  AutocheckWarnings::constantInitializer,
                  Init->getSourceRange())
                  .limitReached();
    }
  }
  return true;
}

bool ConstantInitializer::hasRedeclaration(const clang::VarDecl *VD) const {
  auto Redecl = VD->redecls_begin();
  while (*Redecl != VD)
    std::next(Redecl);
  return std::next(Redecl) != VD->redecls_end();
}

/* Implementation of MoreThanTwoLevelsOfPointerIndirection */

MoreThanTwoLevelsOfPointerIndirection::MoreThanTwoLevelsOfPointerIndirection(
    clang::DiagnosticsEngine &DE, clang::ASTContext &AC)
    : DE(DE), AC(AC) {}

bool MoreThanTwoLevelsOfPointerIndirection::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::pointerIndirectionLevels);
}

bool MoreThanTwoLevelsOfPointerIndirection::hasMoreThanTwoPointerIndirections(
    const clang::QualType QT) {
  if (!QT->isPointerType())
    return false;

  const clang::QualType LevelTwo = QT->getPointeeType();

  if (!LevelTwo->isPointerType())
    return false;

  const clang::QualType LevelThree = LevelTwo->getPointeeType();

  return LevelThree->isPointerType();
}

clang::QualType
MoreThanTwoLevelsOfPointerIndirection::getToPointerIndirectionType(
    clang::QualType QT) {
  if (QT->isReferenceType())
    QT = QT.getNonReferenceType();

  while (const clang::ArrayType *arrayType = QT->getAsArrayTypeUnsafe())
    QT = arrayType->getElementType();

  return QT;
}

const clang::FunctionProtoType *
MoreThanTwoLevelsOfPointerIndirection::getFunctionProtoType(
    clang::QualType QT) {
  while (QT->isPointerType())
    QT = QT->getPointeeType();

  return QT.getTypePtr()->getAs<clang::FunctionProtoType>();
}

bool MoreThanTwoLevelsOfPointerIndirection::checkType(
    clang::QualType QT, const clang::SourceLocation &SL) {
  // If this type is template specalization, check its argument types.
  // For example: std::vector<std::int32_t ***>.
  if (const clang::TemplateSpecializationType *TST =
          QT->getAs<clang::TemplateSpecializationType>()) {

    for (const clang::TemplateArgument &TA : TST->template_arguments()) {
      if (TA.isNull() || (TA.getKind() != clang::TemplateArgument::Type))
        continue;
      if (!checkType(TA.getAsType(), SL))
        return false;
    }
    return true;
  }

  // If this type contains function prototype, get it, and check return value
  // type. Parameter types of this function will be checked by call
  // of chekType function while visiting ParmVarDecl inside VisitVarDecl method.
  if (const clang::FunctionProtoType *FPT = getFunctionProtoType(QT)) {
    if (!checkType(FPT->getReturnType(), SL))
      return false;
  }

  clang::QualType QTIndirection = getToPointerIndirectionType(QT);

  if (!hasMoreThanTwoPointerIndirections(QTIndirection))
    return true;

  return !AutocheckDiagnostic::reportWarning(
              DE, SL, AutocheckWarnings::pointerIndirectionLevels)
              .limitReached();
}

bool MoreThanTwoLevelsOfPointerIndirection::VisitVarDecl(
    const clang::VarDecl *VD) {
  const clang::TypeSourceInfo *TSI = VD->getTypeSourceInfo();

  clang::QualType QT = TSI->getType();

  // If this is function parameter declaration, that means that if this type
  // can decay to pointer, it will. So we have to obtain decayed type.
  if (QT->canDecayToPointerType()) {
    if (llvm::dyn_cast_if_present<const clang::ParmVarDecl>(VD)) {
      QT = AC.getDecayedType(QT);
    }
  }

  return checkType(QT, TSI->getTypeLoc().getBeginLoc());
}

bool MoreThanTwoLevelsOfPointerIndirection::VisitFieldDecl(
    const clang::FieldDecl *FD) {

  const clang::TypeSourceInfo *TSI = FD->getTypeSourceInfo();
  clang::QualType QT = TSI->getType();

  return checkType(QT, TSI->getTypeLoc().getBeginLoc());
}

bool MoreThanTwoLevelsOfPointerIndirection::VisitFunctionDecl(
    const clang::FunctionDecl *FD) {
  clang::QualType QT = FD->getReturnType();
  return checkType(QT, FD->getBeginLoc());
}

/* Implementation of NoExceptionVisitor */

/// Fills SmallSet with all base classes in inheritance chain of given Record.
void getInheritedRecords(
    llvm::SmallSet<const clang::CXXRecordDecl *, 32> &InheritedRecords,
    const clang::CXXRecordDecl *CRD) {
  for (const clang::CXXBaseSpecifier &Base : CRD->bases()) {
    const clang::CXXRecordDecl *BaseRecord =
        Base.getType()->getAsCXXRecordDecl();
    InheritedRecords.insert(BaseRecord);
    if (BaseRecord->bases_begin() != BaseRecord->bases_end())
      getInheritedRecords(InheritedRecords, BaseRecord);
  }
}

/// Get underlying type stripped of pointers.
static const clang::Type *StripPointers(const clang::Type *PT) {
  const clang::Type *T = PT;
  while (T->isPointerType()) {
    T = T->getAs<clang::PointerType>()->getPointeeType().getTypePtr();
  }
  return T;
}

/// Check if type of exception will be caught at the current point in AST.
static bool typeNotCaught(
    const clang::Type *ThrowType,
    llvm::SmallVector<const clang::CXXTryStmt *, 4> &CXXTryStmtStack) {
  // Get non pointer type of thrown exception.
  const clang::Type *NonPointerThrowType = StripPointers(ThrowType);

  // Gather all subclasses in inheritance chain of thrown exception.
  llvm::SmallSet<const clang::CXXRecordDecl *, 32> InheritedRecords;
  if (const clang::CXXRecordDecl *CRD =
          NonPointerThrowType->getAsCXXRecordDecl()) {
    InheritedRecords.insert(CRD);
    getInheritedRecords(InheritedRecords, CRD);
  }

  for (const clang::CXXTryStmt *CTS : CXXTryStmtStack) {
    // Catch handlers are written in order from more specialized types to more
    // general types. This makes it more likely that we will find matching type
    // by looking backwards since we don't need the actual handler that will
    // catch the exception, just whether it will catch it or not.
    for (int i = CTS->getNumHandlers() - 1; i >= 0; --i) {
      const clang::CXXCatchStmt *CCS = CTS->getHandler(i);

      // Check if this is a catch-all statement.
      if (!CCS->getExceptionDecl()) {
        return false;
      }

      // clang::Type of catch statement.
      const clang::Type *CatchType = CCS->getCaughtType().getTypePtr();

      // If this is a reference, strip the reference.
      if (CatchType->isLValueReferenceType()) {
        CatchType = CatchType->getAs<clang::LValueReferenceType>()
                        ->getPointeeType()
                        .getTypePtr();
      }

      // If types match, exception is caught.
      if (ThrowType == CatchType)
        return false;

      // If type of thrown exceptions is a CXXRecord then it can be caught by a
      // reference or pointer to any of it's base classes. All the following
      // checks handle this case.

      // List will be empty for non CXXRecord types.
      if (InheritedRecords.empty())
        continue; // Exception is not caught by this catch statement.

      const clang::Type *ThrowTypeP = ThrowType;

      // There can be multiple level of pointers, but if the exception is to be
      // caught there must be an equal number in both type of exception and
      // type of catch handler.
      // While both are of pointer type, strip pointers.
      while (ThrowTypeP->isPointerType() && CatchType->isPointerType()) {
        ThrowTypeP = ThrowTypeP->getAs<clang::PointerType>()
                         ->getPointeeType()
                         .getTypePtr();
        CatchType = CatchType->getAs<clang::PointerType>()
                        ->getPointeeType()
                        .getTypePtr();
      }

      // If number of pointers is different then these types are not the same.
      if (ThrowTypeP->isPointerType() || CatchType->isPointerType())
        continue; // Exception is not caught by this catch statement.

      // If type of catch statement is also a CXXRecord...
      if (const clang::CXXRecordDecl *CRD = CatchType->getAsCXXRecordDecl()) {
        // ...and it is one of subclasses of exception type...
        if (InheritedRecords.find(CRD) != InheritedRecords.end())
          return false; // ...then the exception is caught.
      }
    }
  }
  return true;
}

static bool noexceptFunction(const clang::FunctionDecl *FD) {
  const clang::FunctionProtoType *Type =
      FD->getType()->getAs<clang::FunctionProtoType>();
  const clang::ExceptionSpecificationType EST = Type->getExceptionSpecType();
  return EST == clang::EST_BasicNoexcept || EST == clang::EST_NoexceptTrue;
}

static bool dependentNoexceptFunction(const clang::FunctionDecl *FD) {
  const clang::FunctionProtoType *Type =
      FD->getType()->getAs<clang::FunctionProtoType>();
  const clang::ExceptionSpecificationType EST = Type->getExceptionSpecType();
  return EST == clang::EST_DependentNoexcept;
}

NoExceptionVisitor::NoExceptionVisitor(clang::DiagnosticsEngine &DE) : DE(DE) {}

bool NoExceptionVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::throwInNoexceptFunc);
}

bool NoExceptionVisitor::VisitCallExpr(const clang::CallExpr *CE) {
  if (!FunctionDeclStack.empty()) {
    const clang::FunctionDecl *FD = CE->getDirectCallee();
    const clang::FunctionDecl *CallerFD = FunctionDeclStack.back();
    if (CallerFD && !CallerFD->isTemplateInstantiation() && FD &&
        (noexceptFunction(CallerFD) || dependentNoexceptFunction(CallerFD)) &&
        !noexceptFunction(FD)) {
      bool stopVisitor =
          AutocheckDiagnostic::reportWarning(
              DE, CE->getBeginLoc(), AutocheckWarnings::throwInNoexceptFunc)
              .limitReached();
      AutocheckDiagnostic::reportWarning(
          DE, CE->getBeginLoc(), AutocheckWarnings::noteThrowInNoexceptFunc, 1);
      DE.Report(CallerFD->getBeginLoc(), clang::diag::note_throw_in_function);

      if (stopVisitor)
        return false;
    }
  }
  return true;
}

bool NoExceptionVisitor::VisitCXXThrowExpr(const clang::CXXThrowExpr *CTE) {
  if (!FunctionDeclStack.empty()) {
    const clang::FunctionDecl *CallerFD = FunctionDeclStack.back();
    if (CallerFD && noexceptFunction(CallerFD)) {
      if (typeNotCaught(CTE->getSubExpr()->getType().getTypePtr(),
                        CXXTryStmtStack)) {
        bool stopVisitor =
            AutocheckDiagnostic::reportWarning(
                DE, CTE->getBeginLoc(), AutocheckWarnings::throwInNoexceptFunc)
                .limitReached();
        AutocheckDiagnostic::reportWarning(
            DE, CTE->getBeginLoc(), AutocheckWarnings::noteThrowInNoexceptFunc,
            0, CallerFD);
        DE.Report(CallerFD->getBeginLoc(), clang::diag::note_throw_in_function);
        if (stopVisitor)
          return false;
      }
    }
  }
  return true;
}

bool NoExceptionVisitor::PreTraverseDecl(clang::Decl *D) {
  if (const clang::FunctionDecl *FD =
          llvm::dyn_cast_if_present<clang::FunctionDecl>(D)) {
    FunctionDeclStack.push_back(FD);
  }
  return true;
}

bool NoExceptionVisitor::PostTraverseDecl(clang::Decl *D) {
  if (!FunctionDeclStack.empty() && FunctionDeclStack.back() == D) {
    FunctionDeclStack.pop_back();
  }
  return true;
}

bool NoExceptionVisitor::PreTraverseStmt(clang::Stmt *S) {
  if (const clang::CXXTryStmt *CTS =
          llvm::dyn_cast_if_present<clang::CXXTryStmt>(S)) {
    CXXTryStmtStack.push_back(CTS);
  }
  return true;
}

bool NoExceptionVisitor::PostTraverseStmt(clang::Stmt *S) {
  if (!CXXTryStmtStack.empty() && CXXTryStmtStack.back() == S) {
    CXXTryStmtStack.pop_back();
  }
  return true;
}

/* Implementation of ExitedWithExceptionVisitor */

ExitedWithExceptionVisitor::ExitedWithExceptionVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool ExitedWithExceptionVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(
      AutocheckWarnings::ctorsDtorsDeallocMoveSwapNoexcept);
}

// Templated move constructors and move assignment operators with dependent
// noexcept are considered exception by Autosar document.
bool ExitedWithExceptionVisitor::isTemplatedWithDependentNoexcept(
    const clang::CXXMethodDecl *MD) {
  if (MD->isTemplated()) {
    const clang::FunctionProtoType *Type =
        MD->getType()->getAs<clang::FunctionProtoType>();
    const clang::ExceptionSpecificationType EST = Type->getExceptionSpecType();
    if (EST == clang::EST_DependentNoexcept)
      return true;
  }
  return false;
}

bool ExitedWithExceptionVisitor::PreTraverseDecl(clang::Decl *D) {
  if (InFunction)
    return true;

  if (const clang::CXXConstructorDecl *CD =
          llvm::dyn_cast_if_present<clang::CXXConstructorDecl>(D)) {
    if (CD->isMoveConstructor()) {
      if (isTemplatedWithDependentNoexcept(CD))
        return true;
      InFunction = CD;
    }
  } else if (const clang::CXXDestructorDecl *DD =
                 llvm::dyn_cast_if_present<clang::CXXDestructorDecl>(D)) {
    InFunction = DD;
  } else if (const clang::CXXMethodDecl *MD =
                 llvm::dyn_cast_if_present<clang::CXXMethodDecl>(D)) {
    if (MD->isMoveAssignmentOperator()) {
      if (isTemplatedWithDependentNoexcept(MD))
        return true;
      InFunction = MD;
    } else if (MD->getOverloadedOperator() == clang::OO_Delete) {
      InFunction = MD;
    }
  } else if (const clang::FunctionDecl *FD =
                 llvm::dyn_cast_if_present<clang::FunctionDecl>(D)) {
    if (isSwapFunction(FD))
      InFunction = FD;
  }
  return true;
}

bool ExitedWithExceptionVisitor::PostTraverseDecl(clang::Decl *D) {
  if (InFunction == D)
    InFunction = nullptr;
  return true;
}

bool ExitedWithExceptionVisitor::PreTraverseStmt(clang::Stmt *S) {
  if (const clang::CXXTryStmt *CTS =
          llvm::dyn_cast_if_present<clang::CXXTryStmt>(S)) {
    CXXTryStmtStack.push_back(CTS);
  }
  return true;
}

bool ExitedWithExceptionVisitor::PostTraverseStmt(clang::Stmt *S) {
  if (!CXXTryStmtStack.empty() && CXXTryStmtStack.back() == S) {
    CXXTryStmtStack.pop_back();
  }
  return true;
}

bool ExitedWithExceptionVisitor::VisitCXXThrowExpr(
    const clang::CXXThrowExpr *CTE) {
  if (InFunction) {
    if (typeNotCaught(CTE->getSubExpr()->getType().getTypePtr(),
                      CXXTryStmtStack)) {
      return !AutocheckDiagnostic::reportWarning(
                  DE, CTE->getBeginLoc(),
                  AutocheckWarnings::ctorsDtorsDeallocMoveSwapNoexcept)
                  .limitReached();
    }
  }
  return true;
}

/* Implementation of MismatchedNewDeleteVisitor */

MismatchedNewDeleteVisitor::MismatchedNewDeleteVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool MismatchedNewDeleteVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::mismatchedNewDelete);
}

bool MismatchedNewDeleteVisitor::VisitVarDecl(const clang::VarDecl *VD) {
  // Skip record types.
  if (VD->getType()->isRecordType())
    return true;

  // Collect pointer variables initialized with operator 'new'.
  if (const clang::Expr *InitExpr = VD->getInit()) {
    std::pair<const clang::CXXNewExpr *, const clang::CXXNewExpr *> NewPair =
        getNewExpr(InitExpr);
    if (NewPair.first) {
      if (NewPair.second) {
        return reportWarning(NewPair.first, NewPair.second);
      } else
        VarDeclsWithNew.insert(std::make_pair(VD, NewPair.first));
    }
  }
  return true;
}

bool MismatchedNewDeleteVisitor::VisitBinaryOperator(
    const clang::BinaryOperator *BO) {
  // Handle the case of the assignment operator where the pointer variable is
  // (re)initalized with operator 'new'.
  if (BO->isAssignmentOp()) {
    std::pair<const clang::CXXNewExpr *, const clang::CXXNewExpr *> NewPair =
        getNewExpr(BO->getRHS());
    if (NewPair.first) {
      if (NewPair.second) {
        return reportWarning(NewPair.first, NewPair.second);
      } else if (const clang::CXXNewExpr *PrevNew = checkMismatch(
                     BO->getLHS(), NewPair.first, NewPair.first->isArray()))
        return reportWarning(PrevNew, NewPair.first);
    }
  }
  return true;
}

bool MismatchedNewDeleteVisitor::VisitCXXDeleteExpr(
    const clang::CXXDeleteExpr *CDE) {
  // Check if operator 'delete' is used on a variable allocated with matching
  // operator 'new'.
  const clang::Expr *Arg = CDE->getArgument()->IgnoreImpCasts();
  if (const clang::CXXNewExpr *PrevNew =
          checkMismatch(Arg, CDE, CDE->isArrayForm()))
    return reportWarning(PrevNew, CDE);
  return true;
}

bool MismatchedNewDeleteVisitor::VisitCXXRecordDecl(
    const clang::CXXRecordDecl *CRD) {
  // Collect pointer fields with 'new'.
  for (const clang::FieldDecl *FD : CRD->fields()) {
    // Skip record types.
    if (FD->getType()->isRecordType())
      continue;

    if (const clang::Expr *Init = FD->getInClassInitializer()) {
      std::pair<const clang::CXXNewExpr *, const clang::CXXNewExpr *> NewPair =
          getNewExpr(Init);
      if (NewPair.first) {
        if (NewPair.second) {
          if (!reportWarning(NewPair.first, NewPair.second))
            return false;
        } else
          FieldsWithNew.insert(std::make_pair(FD, NewPair.first));
      }
    }
  }

  // Check for pointer field initalizations in constructors.
  for (const clang::CXXConstructorDecl *CCD : CRD->ctors()) {
    for (const clang::CXXCtorInitializer *CCI : CCD->inits()) {
      std::pair<const clang::CXXNewExpr *, const clang::CXXNewExpr *> NewPair =
          getNewExpr(CCI->getInit());
      if (NewPair.first) {
        if (NewPair.second) {
          if (!reportWarning(NewPair.first, NewPair.second))
            return false;
        } else {
          if (const clang::CXXNewExpr *Prev =
                  checkDeclMismatch(FieldsWithNew, CCI->getMember(),
                                    NewPair.first, NewPair.first->isArray()))
            if (!reportWarning(Prev, NewPair.first))
              return false;
        }
      }
    }
  }
  return true;
}

const clang::CXXNewExpr *MismatchedNewDeleteVisitor::checkMismatch(
    const clang::Expr *E, const clang::Expr *NewOrDelete, bool isArray) {
  assert((llvm::isa<clang::CXXNewExpr>(NewOrDelete) ||
          llvm::isa<clang::CXXDeleteExpr>(NewOrDelete)) &&
         "second argument must be a 'new' or 'delete' expression");

  // If this is an array subscript operator then get the underneath pointer.
  const clang::Expr *SubExpr = ignoreArraySubscript(E);

  // Check for pointer fields.
  if (const clang::MemberExpr *ME =
          llvm::dyn_cast_if_present<clang::MemberExpr>(SubExpr)) {
    if (const clang::FieldDecl *FD =
            llvm::dyn_cast_if_present<clang::FieldDecl>(ME->getMemberDecl()))
      return checkDeclMismatch(FieldsWithNew, FD, NewOrDelete, isArray);
  }
  // Check for pointer variables.
  else if (const clang::DeclRefExpr *DRE =
               llvm::dyn_cast_if_present<clang::DeclRefExpr>(SubExpr)) {
    if (const clang::VarDecl *VD =
            llvm::dyn_cast_if_present<clang::VarDecl>(DRE->getDecl()))
      return checkDeclMismatch(VarDeclsWithNew, VD, NewOrDelete, isArray);
  }
  return nullptr;
}

template <class T>
const clang::CXXNewExpr *MismatchedNewDeleteVisitor::checkDeclMismatch(
    std::map<const T *, const clang::CXXNewExpr *> &DeclsWithNew, const T *Decl,
    const clang::Expr *NewOrDelete, bool isArray) {
  // If this Decl was previously initialized with operator 'new' ...
  auto MapIt = DeclsWithNew.find(Decl);
  if (MapIt != DeclsWithNew.end()) {
    // ... then check if operator types match.
    if (MapIt->second->isArray() != isArray) {
      const clang::CXXNewExpr *PrevNew = MapIt->second;
      DeclsWithNew.erase(MapIt);
      return PrevNew;
    }
  }
  // Collect pointer variables with 'new'.
  else if (const clang::CXXNewExpr *CNE =
               llvm::dyn_cast_if_present<clang::CXXNewExpr>(NewOrDelete))
    DeclsWithNew.insert(std::make_pair(Decl, CNE));
  return nullptr;
}

const clang::Expr *
MismatchedNewDeleteVisitor::ignoreArraySubscript(const clang::Expr *E) {
  while (const clang::ArraySubscriptExpr *Ar =
             llvm::dyn_cast_if_present<clang::ArraySubscriptExpr>(E))
    E = Ar->getLHS()->IgnoreImpCasts();
  return E;
}

std::pair<const clang::CXXNewExpr *, const clang::CXXNewExpr *>
MismatchedNewDeleteVisitor::getNewExpr(const clang::Stmt *Init,
                                       const clang::CXXNewExpr *FirstNew) {
  // Init is a single CXXNewExpr.
  if (const clang::CXXNewExpr *CNE =
          llvm::dyn_cast_if_present<clang::CXXNewExpr>(Init)) {
    if (!FirstNew)
      return std::make_pair(CNE,
                            nullptr); // This is the first CXXNewExpr.
    if (FirstNew->isArray() != CNE->isArray())
      return std::make_pair(FirstNew, CNE); // Mismatch 'new' and 'new[]'.
  }
  // If this is an InitList, check all lists for mismatch.
  else if (const clang::InitListExpr *ILE =
               llvm::dyn_cast_if_present<clang::InitListExpr>(Init)) {
    for (const clang::Stmt *Child : ILE->children()) {
      std::pair<const clang::CXXNewExpr *, const clang::CXXNewExpr *> Res =
          getNewExpr(Child, FirstNew);
      if (Res.second)         // Has a mismatch...
        return Res;           // ...exit early.
      else if (!FirstNew)     // If there was no first yet...
        FirstNew = Res.first; // ...update first.
    }
  }
  // No CXXNewExpr of InitListExpr found.
  return std::make_pair(FirstNew, nullptr);
}

bool MismatchedNewDeleteVisitor::reportWarning(const clang::CXXNewExpr *Prev,
                                               const clang::Expr *Curr) {
  bool LimitReached =
      AutocheckDiagnostic::reportWarning(DE, Curr->getBeginLoc(),
                                         AutocheckWarnings::mismatchedNewDelete)
          .limitReached();
  AutocheckDiagnostic::reportWarning(
      DE, Curr->getBeginLoc(), AutocheckWarnings::noteMismatchedNewDelete,
      llvm::isa<clang::CXXNewExpr>(Curr), !Prev->isArray());
  DE.Report(Prev->getBeginLoc(), clang::diag::note_allocated_here)
      << !Prev->isArray();
  return !LimitReached;
}

/* Implementation of CStyleStringUsed */

CStyleStringUsed::CStyleStringUsed(clang::DiagnosticsEngine &DE,
                                   clang::ASTContext &AC)
    : DE(DE), AC(AC) {}

bool CStyleStringUsed::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::cStyleStringUsed);
}

bool CStyleStringUsed::VisitDeclRefExpr(const clang::DeclRefExpr *DRE) {
  // Prohibit assignment of string literals to variables of type char* or
  // char[]
  if (isCharPtrOrArray(DRE->getDecl()->getType())) {
    const auto &Parents = AC.getParents(*DRE);
    if (const clang::BinaryOperator *BOP =
            Parents[0].get<clang::BinaryOperator>()) {
      if (BOP->isAssignmentOp()) {
        if (llvm::dyn_cast_if_present<clang::StringLiteral>(
                BOP->getRHS()->IgnoreCasts())) {
          return !AutocheckDiagnostic::reportWarning(
                      DE, DRE->getBeginLoc(),
                      AutocheckWarnings::cStyleStringUsed)
                      .limitReached();
        }
      }
    }
  }

  return true;
}

bool CStyleStringUsed::isCharPtrOrArray(const clang::QualType &T) const {
  return (T->isPointerType() && T->getPointeeType()->isCharType()) ||
         (T->isArrayType() &&
          T->getAsArrayTypeUnsafe()->getElementType()->isCharType());
}

bool CStyleStringUsed::VisitVarDecl(const clang::VarDecl *VD) {
  if (llvm::dyn_cast_if_present<clang::ParmVarDecl>(VD)) {
    // Ignore function parameters since there is no way to deduce if they are
    // intended to be used as c-style strings at this point.
    return true;
  }

  if (isCharPtrOrArray(VD->getCanonicalDecl()->getType())) {

    if (VD->hasInit()) {
      // Variable initialized using c-style string.
      if (llvm::dyn_cast_if_present<clang::StringLiteral>(
              VD->getInit()->IgnoreCasts())) {
        return !AutocheckDiagnostic::reportWarning(
                    DE, VD->getBeginLoc(), AutocheckWarnings::cStyleStringUsed)
                    .limitReached();
      } // Variable initialized using list initialization.
      else if (const clang::InitListExpr *ILE =
                   llvm::dyn_cast_if_present<clang::InitListExpr>(
                       VD->getInit()->IgnoreCasts())) {

        if (ILE->inits().empty())
          return true;

        auto lastElement = ILE->inits().back()->IgnoreCasts();
        // Checks for strings like char* str = {'h', 'i', '\0'}.
        if (const clang::CharacterLiteral *CL =
                llvm::dyn_cast_if_present<clang::CharacterLiteral>(
                    lastElement)) {
          if (CL->getValue() == '\0') {
            return !AutocheckDiagnostic::reportWarning(
                        DE, VD->getBeginLoc(),
                        AutocheckWarnings::cStyleStringUsed)
                        .limitReached();
          }
        } // Checks for strings like char* str{"hi"}.
        else if (llvm::dyn_cast_if_present<clang::StringLiteral>(lastElement)) {
          return !AutocheckDiagnostic::reportWarning(
                      DE, VD->getBeginLoc(),
                      AutocheckWarnings::cStyleStringUsed)
                      .limitReached();
        }
      }
    } else {
      // This variable could be used as a C-style string in the future.
      PotentiallyBadVars.insert(VD);
    }
  }

  return true;
}

bool CStyleStringUsed::VisitCallExpr(const clang::CallExpr *CE) {
  // All built-in functions that accept char* or char[] arguments are
  // forbidden.
  if (const clang::FunctionDecl *FD = CE->getDirectCallee()) {
    clang::SourceManager &SM = AC.getSourceManager();
    if (SM.isInSystemHeader(FD->getBeginLoc())) {
      // Prohibit usage of string class's c_str method.
      if (FD->getNameInfo().getAsString() == "c_str") {
        return !AutocheckDiagnostic::reportWarning(
                    DE, CE->getBeginLoc(), AutocheckWarnings::cStyleStringUsed)
                    .limitReached();
      }
      // Allow usage of memory manipulation functions since they are working
      // with buffers, and not c-style strings.
      std::vector<std::string> memoryFuncs{"memchr", "memcmp", "memset",
                                           "memcpy", "memmove"};
      std::string name = FD->getNameInfo().getAsString();
      if (std::find(memoryFuncs.begin(), memoryFuncs.end(), name) !=
          memoryFuncs.end()) {
        return true;
      }

      for (unsigned i = 0; i < CE->getNumArgs(); i++) {
        const clang::Expr *Arg = CE->getArg(i)->IgnoreImpCasts();
        if (const clang::DeclRefExpr *DRE =
                llvm::dyn_cast_if_present<clang::DeclRefExpr>(Arg)) {
          auto F = PotentiallyBadVars.find(DRE->getDecl());
          if (F != PotentiallyBadVars.end()) {
            PotentiallyBadVars.erase(DRE->getDecl());
            return !AutocheckDiagnostic::reportWarning(
                        DE, DRE->getDecl()->getBeginLoc(),
                        AutocheckWarnings::cStyleStringUsed)
                        .limitReached();
          }
        }
      }
    }
  }

  return true;
}

/* Implementation of ExternArrayImplicitSizeVisitor */

ExternArrayImplicitSizeVisitor::ExternArrayImplicitSizeVisitor(
    clang::DiagnosticsEngine &DE, clang::ASTContext &AC)
    : DE(DE), AC(AC) {}

bool ExternArrayImplicitSizeVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::externArrayImplicitSize);
}

bool ExternArrayImplicitSizeVisitor::VisitVarDecl(const clang::VarDecl *VD) {
  // Early return if the variable isn't declared extern.
  if (!VD->hasExternalStorage())
    return true;

  if (const clang::IncompleteArrayType *IAT =
          llvm::dyn_cast_if_present<clang::IncompleteArrayType>(
              VD->getType())) {
    // Case 1: The array size is unknown.
    // Example: int a[];
    return !AutocheckDiagnostic::reportWarning(
                DE, VD->getLocation(),
                AutocheckWarnings::externArrayImplicitSize)
                .limitReached();
  } else if (const clang::ConstantArrayType *CAT =
                 llvm::dyn_cast_if_present<clang::ConstantArrayType>(
                     VD->getType())) {
    // Case 2: The array size is known, but not explicitly written (e.g. deduced
    // from initialization).
    // Example: int b[] = {1, 2, 3};
    if (!VD->getTypeSourceInfo()
             ->getTypeLoc()
             .getAs<clang::IncompleteArrayTypeLoc>()
             .isNull()) {
      return !AutocheckDiagnostic::reportWarning(
                  DE, VD->getLocation(),
                  AutocheckWarnings::externArrayImplicitSize)
                  .limitReached();
    }
  }
  return true;
}

/* Implementation of TypedefUsedVisitor */

TypedefUsedVisitor::TypedefUsedVisitor(clang::DiagnosticsEngine &DE) : DE(DE) {}

bool TypedefUsedVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::typedefUsed);
}

bool TypedefUsedVisitor::VisitTypedefDecl(const clang::TypedefDecl *TD) {
  return !AutocheckDiagnostic::reportWarning(DE, TD->getBeginLoc(),
                                             AutocheckWarnings::typedefUsed)
              .limitReached();
}

/* Implementation of FriendDeclUsedVisitor */

FriendDeclUsedVisitor::FriendDeclUsedVisitor(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool FriendDeclUsedVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::friendUsed);
}

bool FriendDeclUsedVisitor::VisitFriendDecl(const clang::FriendDecl *FD) {
  return !AutocheckDiagnostic::reportWarning(DE, FD->getLocation(),
                                             AutocheckWarnings::friendUsed)
              .limitReached();
}

/* Implementation of EnumTypeNotDefinedVisitor */

EnumTypeNotDefinedVisitor::EnumTypeNotDefinedVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool EnumTypeNotDefinedVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::enumTypeNotDefined);
}

bool EnumTypeNotDefinedVisitor::VisitEnumDecl(const clang::EnumDecl *ED) {
  if (!ED->getIntegerTypeSourceInfo()) {
    return !AutocheckDiagnostic::reportWarning(
                DE, ED->getLocation(), AutocheckWarnings::enumTypeNotDefined)
                .limitReached();
  }
  return true;
}

/* Implementation of BlockScopeFunctionVisitor */

BlockScopeFunctionVisitor::BlockScopeFunctionVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool BlockScopeFunctionVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::functionAtBlockScope);
}

bool BlockScopeFunctionVisitor::VisitFunctionDecl(
    const clang::FunctionDecl *FD) {
  if (FD->getLexicalDeclContext()->isFunctionOrMethod()) {
    return !AutocheckDiagnostic::reportWarning(
                DE, FD->getLocation(), AutocheckWarnings::functionAtBlockScope)
                .limitReached();
  }
  return true;
}

/* Implementation of StaticFunctionRedeclarationVisitor */

StaticFunctionRedeclarationVisitor::StaticFunctionRedeclarationVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool StaticFunctionRedeclarationVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::staticFunctionRedeclaration);
}

bool StaticFunctionRedeclarationVisitor::VisitFunctionDecl(
    const clang::FunctionDecl *FD) {
  // If the function is already marked static, or if this is the first
  // declaration there is nothing to do.
  if (FD->getStorageClass() == clang::StorageClass::SC_Static ||
      FD->getFirstDecl() == FD)
    return true;

  // Ignore method declarations.
  if (llvm::dyn_cast_if_present<clang::CXXMethodDecl>(FD->getFirstDecl()))
    return true;

  // Iterate through all previous declarations
  for (auto It = ++FD->redecls_begin(); It != FD->redecls_end(); ++It) {
    if (llvm::dyn_cast_if_present<clang::CXXMethodDecl>(FD))
      return true;

    if (It->getLinkageAndVisibility().getLinkage() ==
            clang::Linkage::InternalLinkage &&
        It->getStorageClass() == clang::SC_Static) {
      return !AutocheckDiagnostic::reportWarning(
                  DE, FD->getLocation(),
                  AutocheckWarnings::staticFunctionRedeclaration)
                  .limitReached();
    }

    // If this was the first declaration, stop iterating.
    if (*It == FD->getFirstDecl())
      break;
  }

  return true;
}

/* Implementation of EnumDeclaredScopedVisitor */

EnumDeclaredScopedVisitor::EnumDeclaredScopedVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool EnumDeclaredScopedVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::enumNotScopedEnumClass);
}

bool EnumDeclaredScopedVisitor::VisitEnumDecl(const clang::EnumDecl *ED) {
  if (!ED->isScoped()) {
    return !AutocheckDiagnostic::reportWarning(
                DE, ED->getLocation(),
                AutocheckWarnings::enumNotScopedEnumClass)
                .limitReached();
  }
  return true;
}

/* Implementation of EnumConstantInitVisitor */

EnumConstantInitVisitor::EnumConstantInitVisitor(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool EnumConstantInitVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::enumeratorInitialization);
}

static bool isInitialized(const clang::EnumConstantDecl *ECD) {
  return ECD->getInitExpr() != nullptr;
}

bool EnumConstantInitVisitor::VisitEnumDecl(const clang::EnumDecl *ED) {
  // Skip forward declarations.
  if (!ED->isCompleteDefinition())
    return true;

  // If there are less than 2 constants, the initialization is compliant.
  if (std::distance(ED->enumerator_begin(), ED->enumerator_end()) < 2)
    return true;

  bool IsFirstInitialized = isInitialized(*ED->enumerator_begin());
  if (IsFirstInitialized) {
    // The first element was initialized, so either all other have to be
    // initiazlied, or none of them.
    clang::EnumDecl::enumerator_iterator Current = ++ED->enumerator_begin();
    bool ShouldAllBeInitialized = isInitialized(*Current);
    for (; Current != ED->enumerator_end(); ++Current) {
      if (isInitialized(*Current) != ShouldAllBeInitialized) {
        return !AutocheckDiagnostic::reportWarning(
                    DE, ED->getLocation(),
                    AutocheckWarnings::enumeratorInitialization)
                    .limitReached();
      }
    }
  } else {
    // The first element wasn't initialized, so other musn't be as well.
    if (std::any_of(++ED->enumerator_begin(), ED->enumerator_end(),
                    [](const clang::EnumConstantDecl *ECD) {
                      return ECD->getInitExpr() != nullptr;
                    })) {
      return !AutocheckDiagnostic::reportWarning(
                  DE, ED->getLocation(),
                  AutocheckWarnings::enumeratorInitialization)
                  .limitReached();
    }
  }

  return true;
}

/* Implementation of MainReusedVisitor */

MainReusedVisitor::MainReusedVisitor(clang::DiagnosticsEngine &DE) : DE(DE) {}

bool MainReusedVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::identifierMainReused);
}

bool MainReusedVisitor::VisitFunctionDecl(const clang::FunctionDecl *FD) {
  if (FD->getIdentifier() && (FD->getName() == "main") && !FD->isMain())
    return !AutocheckDiagnostic::reportWarning(
                DE, FD->getNameInfo().getLoc(),
                AutocheckWarnings::identifierMainReused)
                .limitReached();

  return true;
}

/* Implementation of UnnamedNamespaceInHeaderVisitor */

UnnamedNamespaceInHeaderVisitor::UnnamedNamespaceInHeaderVisitor(
    clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool UnnamedNamespaceInHeaderVisitor::isFlagPresent(
    const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::unnamedNamespaceInHeader);
}

bool UnnamedNamespaceInHeaderVisitor::VisitNamespaceDecl(
    const clang::NamespaceDecl *ND) {
  if (ND->isAnonymousNamespace() &&
      !DE.getSourceManager().isInMainFile(ND->getBeginLoc()))
    return !AutocheckDiagnostic::reportWarning(
                DE, ND->getBeginLoc(),
                AutocheckWarnings::unnamedNamespaceInHeader)
                .limitReached();

  return true;
}

/* Implementation of UsingDirectiveVisitor */

UsingDirectiveVisitor::UsingDirectiveVisitor(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool UsingDirectiveVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::usingDirectiveUsed);
}

bool UsingDirectiveVisitor::VisitUsingDirectiveDecl(
    const clang::UsingDirectiveDecl *UDD) {
  return !AutocheckDiagnostic::reportWarning(
              DE, UDD->getLocation(), AutocheckWarnings::usingDirectiveUsed)
              .limitReached();
}

/* Implementation of GlobalNamespaceVisitor */

GlobalNamespaceVisitor::GlobalNamespaceVisitor(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool GlobalNamespaceVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::globalNamespaceNodes);
}

bool GlobalNamespaceVisitor::VisitTranslationUnitDecl(
    const clang::TranslationUnitDecl *TUD) {
  for (auto It = TUD->decls_begin(); It != TUD->decls_end(); ++It) {
    // Skip typedefs.
    if (clang::TypedefDecl *TD =
            llvm::dyn_cast_if_present<clang::TypedefDecl>(*It)) {
      // Skip built-in typedefs.
      if (!TD->getBeginLoc().isValid())
        continue;
      // Check allowed typedefs.
      const auto &It = AllowedTypedefs.find(TD->getName().str());
      if (It != AllowedTypedefs.end())
        if (It->second == TD->getUnderlyingType().getAsString())
          continue;
      // Skip namespaces and extern "C".
    } else if (llvm::dyn_cast_if_present<clang::NamespaceDecl>(*It) ||
               llvm::dyn_cast_if_present<clang::LinkageSpecDecl>(*It)) {
      continue;
      // Skip implicit using directives.
    } else if (clang::UsingDirectiveDecl *UDD =
                   llvm::dyn_cast_if_present<clang::UsingDirectiveDecl>(*It)) {
      if (UDD->getNominatedNamespace()->isAnonymousNamespace())
        continue;
      // Skip using shadow declarations.
    } else if (llvm::dyn_cast_if_present<clang::UsingShadowDecl>(*It)) {
      continue;
      // Skip main function.
    } else if (clang::FunctionDecl *FD =
                   llvm::dyn_cast_if_present<clang::FunctionDecl>(*It)) {
      if (FD->isMain())
        continue;
    }
    if (It->getBeginLoc().isInvalid())
      continue;
    if (AutocheckDiagnostic::reportWarning(
            DE, It->getBeginLoc(), AutocheckWarnings::globalNamespaceNodes)
            .limitReached()) {
      return false;
    }
  }

  return true;
}

const std::map<std::string, std::string>
    GlobalNamespaceVisitor::AllowedTypedefs{{"char_t", "char"},
                                            {"int8_t", "signed char"},
                                            {"int16_t", "short"},
                                            {"int32_t", "int"},
                                            {"int64_t", "long"},
                                            {"uint8_t", "unsigned char"},
                                            {"uint16_t", "unsigned short"},
                                            {"uint32_t", "unsigned int"},
                                            {"uint64_t", "unsigned long"},
                                            {"float32_t", "float"},
                                            {"float64_t", "double"},
                                            {"float128_t", "long double"}};

/* Implementation of BitFieldTypeVisitor */

BitFieldTypeVisitor::BitFieldTypeVisitor(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

bool BitFieldTypeVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::bitFieldUnsignedIntegral);
}

static bool isUnsignedIntegerType(clang::QualType Type) {
  return Type->isUnsignedIntegerType() && !Type->isBooleanType();
}

static bool isAllowedBitfieldType(const clang::QualType &Type) {
  clang::QualType CanonicalType = Type.getCanonicalType();

  // Check is builtin unsigned type.
  if (CanonicalType->isBuiltinType())
    return isUnsignedIntegerType(CanonicalType);

  // Check is enum type with explicitly written unsigned underlying type.
  if (const auto *ET = llvm::dyn_cast<clang::EnumType>(CanonicalType)) {
    clang::EnumDecl *ED = ET->getDecl();

    return ED->isComplete() && ED->getIntegerTypeSourceInfo() &&
           isUnsignedIntegerType(ED->getIntegerTypeSourceInfo()->getType());
  }

  return false;
}

bool BitFieldTypeVisitor::VisitFieldDecl(const clang::FieldDecl *FD) {
  if (FD->isBitField() && !isAllowedBitfieldType(FD->getType()))
    return !AutocheckDiagnostic::reportWarning(
                DE, FD->getBeginLoc(),
                AutocheckWarnings::bitFieldUnsignedIntegral)
                .limitReached();

  return true;
}

/* DeclarationsVisitor */

DeclarationsVisitor::DeclarationsVisitor(clang::DiagnosticsEngine &DE,
                                         clang::ASTContext &AC,
                                         clang::Sema &SemaRef)
    : DE(DE) {
  const AutocheckContext &Context = AutocheckContext::Get();
  if (ASMDeclarationUsedVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<ASMDeclarationUsedVisitor>(DE));
  if (VariadicFunctionUsedVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<VariadicFunctionUsedVisitor>(DE));
  if (FunctionRedeclParamsVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<FunctionRedeclParamsVisitor>(DE));
  if (AutoVarBracedInitVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<AutoVarBracedInitVisitor>(DE, SemaRef));
  if (VarBracedInitVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<VarBracedInitVisitor>(DE, AC, SemaRef));
  if (UnusedGlobalTypedef::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<UnusedGlobalTypedef>(DE));
  if (CVQualifiersPlacedLeftVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<CVQualifiersPlacedLeftVisitor>(
        DE.getSourceManager(), DE, SemaRef));
  if (AssignmentOperatorReturn::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<AssignmentOperatorReturn>(DE));
  if (ConstUnusedForImmutableData::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<ConstUnusedForImmutableData>(DE, AC));
  if (NSDMIAndConsutructorInitUsed::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<NSDMIAndConsutructorInitUsed>(DE));
  if (BroadScopeIdentifierVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<BroadScopeIdentifierVisitor>(DE, AC));
  if (MissingBracesOrEltsVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<MissingBracesOrEltsVisitor>(DE, SemaRef));
  if (ConstantInitializer::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<ConstantInitializer>(DE, AC, SemaRef));
  if (RandomEngineDefaultInitialized::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<RandomEngineDefaultInitialized>(DE));
  if (MoreThanTwoLevelsOfPointerIndirection::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<MoreThanTwoLevelsOfPointerIndirection>(DE, AC));
  if (NoExceptionVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<NoExceptionVisitor>(DE));
  if (ExitedWithExceptionVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<ExitedWithExceptionVisitor>(DE));
  if (MismatchedNewDeleteVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<MismatchedNewDeleteVisitor>(DE));
  if (CStyleStringUsed::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<CStyleStringUsed>(DE, AC));
  if (ExternArrayImplicitSizeVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<ExternArrayImplicitSizeVisitor>(DE, AC));
  if (TypedefUsedVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<TypedefUsedVisitor>(DE));
  if (FriendDeclUsedVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<FriendDeclUsedVisitor>(DE));
  if (EnumTypeNotDefinedVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<EnumTypeNotDefinedVisitor>(DE));
  if (BlockScopeFunctionVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<BlockScopeFunctionVisitor>(DE));
  if (StaticFunctionRedeclarationVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<StaticFunctionRedeclarationVisitor>(DE));
  if (EnumDeclaredScopedVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<EnumDeclaredScopedVisitor>(DE));
  if (EnumConstantInitVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<EnumConstantInitVisitor>(DE));
  if (MainReusedVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<MainReusedVisitor>(DE));
  if (UnnamedNamespaceInHeaderVisitor::isFlagPresent(Context))
    AllVisitors.push_front(
        std::make_unique<UnnamedNamespaceInHeaderVisitor>(DE));
  if (UsingDirectiveVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<UsingDirectiveVisitor>(DE));
  if (GlobalNamespaceVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<GlobalNamespaceVisitor>(DE));
  if (BitFieldTypeVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<BitFieldTypeVisitor>(DE));
}

void DeclarationsVisitor::run(clang::TranslationUnitDecl *TUD) {
  if (!AllVisitors.empty()) {
    this->TraverseDecl(TUD);
    for (auto &Visitor : AllVisitors)
      Visitor->PostWork();
  }
}

bool DeclarationsVisitor::TraverseDecl(clang::Decl *D) {
  if (!D) // Can be null if code had errors.
    return true;

  clang::SourceLocation Loc = D->getBeginLoc();

  if (Loc.isInvalid() || appropriateHeaderLocation(DE, Loc)) {
    AllVisitors.remove_if(
        [D](std::unique_ptr<DeclarationsVisitorInterface> &V) {
          return !V->PreTraverseDecl(D);
        });

    clang::RecursiveASTVisitor<DeclarationsVisitor>::TraverseDecl(D);

    AllVisitors.remove_if(
        [D](std::unique_ptr<DeclarationsVisitorInterface> &V) {
          return !V->PostTraverseDecl(D);
        });
  }
  return true;
}

bool DeclarationsVisitor::TraverseStmt(clang::Stmt *S) {
  AllVisitors.remove_if([S](std::unique_ptr<DeclarationsVisitorInterface> &V) {
    return !V->PreTraverseStmt(S);
  });

  clang::RecursiveASTVisitor<DeclarationsVisitor>::TraverseStmt(S);

  AllVisitors.remove_if([S](std::unique_ptr<DeclarationsVisitorInterface> &V) {
    return !V->PostTraverseStmt(S);
  });

  return true;
}

bool DeclarationsVisitor::VisitGCCAsmStmt(const clang::GCCAsmStmt *GAS) {
  AllVisitors.remove_if(
      [GAS](std::unique_ptr<DeclarationsVisitorInterface> &V) {
        return !V->VisitGCCAsmStmt(GAS);
      });
  return true;
}

bool DeclarationsVisitor::VisitVarDecl(const clang::VarDecl *VD) {
  AllVisitors.remove_if([VD](std::unique_ptr<DeclarationsVisitorInterface> &V) {
    return !V->VisitVarDecl(VD);
  });
  return true;
}

bool DeclarationsVisitor::VisitFunctionDecl(const clang::FunctionDecl *FD) {
  AllVisitors.remove_if([FD](std::unique_ptr<DeclarationsVisitorInterface> &V) {
    return !V->VisitFunctionDecl(FD);
  });
  return true;
}

bool DeclarationsVisitor::VisitTypedefNameDecl(
    const clang::TypedefNameDecl *TND) {
  AllVisitors.remove_if(
      [TND](std::unique_ptr<DeclarationsVisitorInterface> &V) {
        return !V->VisitTypedefNameDecl(TND);
      });
  return true;
}

bool DeclarationsVisitor::VisitCXXMethodDecl(const clang::CXXMethodDecl *CMD) {
  AllVisitors.remove_if(
      [CMD](std::unique_ptr<DeclarationsVisitorInterface> &V) {
        return !V->VisitCXXMethodDecl(CMD);
      });
  return true;
}

bool DeclarationsVisitor::VisitUnaryOperator(const clang::UnaryOperator *UO) {
  AllVisitors.remove_if([UO](std::unique_ptr<DeclarationsVisitorInterface> &V) {
    return !V->VisitUnaryOperator(UO);
  });
  return true;
}

bool DeclarationsVisitor::VisitBinaryOperator(const clang::BinaryOperator *BO) {
  AllVisitors.remove_if([BO](std::unique_ptr<DeclarationsVisitorInterface> &V) {
    return !V->VisitBinaryOperator(BO);
  });
  return true;
}

bool DeclarationsVisitor::VisitCallExpr(const clang::CallExpr *CE) {
  AllVisitors.remove_if([CE](std::unique_ptr<DeclarationsVisitorInterface> &V) {
    return !V->VisitCallExpr(CE);
  });
  return true;
}

bool DeclarationsVisitor::VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) {
  AllVisitors.remove_if(
      [CRD](std::unique_ptr<DeclarationsVisitorInterface> &V) {
        return !V->VisitCXXRecordDecl(CRD);
      });
  return true;
}

bool DeclarationsVisitor::VisitFieldDecl(const clang::FieldDecl *FD) {
  AllVisitors.remove_if([FD](std::unique_ptr<DeclarationsVisitorInterface> &V) {
    return !V->VisitFieldDecl(FD);
  });
  return true;
}

bool DeclarationsVisitor::VisitDeclRefExpr(const clang::DeclRefExpr *DRE) {
  AllVisitors.remove_if(
      [DRE](std::unique_ptr<DeclarationsVisitorInterface> &V) {
        return !V->VisitDeclRefExpr(DRE);
      });
  return true;
}

bool DeclarationsVisitor::VisitCXXConstructExpr(
    const clang::CXXConstructExpr *CCE) {
  AllVisitors.remove_if(
      [CCE](std::unique_ptr<DeclarationsVisitorInterface> &V) {
        return !V->VisitCXXConstructExpr(CCE);
      });
  return true;
}

bool DeclarationsVisitor::VisitCXXThrowExpr(const clang::CXXThrowExpr *CTE) {
  AllVisitors.remove_if(
      [CTE](std::unique_ptr<DeclarationsVisitorInterface> &V) {
        return !V->VisitCXXThrowExpr(CTE);
      });
  return true;
}

bool DeclarationsVisitor::VisitCXXDeleteExpr(const clang::CXXDeleteExpr *CDE) {
  AllVisitors.remove_if(
      [CDE](std::unique_ptr<DeclarationsVisitorInterface> &V) {
        return !V->VisitCXXDeleteExpr(CDE);
      });
  return true;
}

bool DeclarationsVisitor::VisitTypedefDecl(const clang::TypedefDecl *TD) {
  AllVisitors.remove_if([TD](std::unique_ptr<DeclarationsVisitorInterface> &V) {
    return !V->VisitTypedefDecl(TD);
  });
  return true;
}

bool DeclarationsVisitor::VisitFriendDecl(const clang::FriendDecl *FD) {
  AllVisitors.remove_if([FD](std::unique_ptr<DeclarationsVisitorInterface> &V) {
    return !V->VisitFriendDecl(FD);
  });
  return true;
}

bool DeclarationsVisitor::VisitEnumDecl(const clang::EnumDecl *ED) {
  AllVisitors.remove_if([ED](std::unique_ptr<DeclarationsVisitorInterface> &V) {
    return !V->VisitEnumDecl(ED);
  });
  return true;
}

bool DeclarationsVisitor::VisitNamespaceDecl(const clang::NamespaceDecl *ND) {
  AllVisitors.remove_if([ND](std::unique_ptr<DeclarationsVisitorInterface> &V) {
    return !V->VisitNamespaceDecl(ND);
  });
  return true;
}

bool DeclarationsVisitor::VisitUsingDirectiveDecl(
    const clang::UsingDirectiveDecl *UDD) {
  AllVisitors.remove_if(
      [UDD](std::unique_ptr<DeclarationsVisitorInterface> &V) {
        return !V->VisitUsingDirectiveDecl(UDD);
      });
  return true;
}

bool DeclarationsVisitor::VisitTranslationUnitDecl(
    const clang::TranslationUnitDecl *TUD) {
  AllVisitors.remove_if(
      [TUD](std::unique_ptr<DeclarationsVisitorInterface> &V) {
        return !V->VisitTranslationUnitDecl(TUD);
      });
  return true;
}

} // namespace autocheck
