//===--- SideEffectChecker.cpp - Expression side effect checker -----------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines helper functions to determine whether a given expression
// has side effects according to AUTOSAR rules.
//
//===----------------------------------------------------------------------===//

#include "AST/SideEffectChecker.h"

#include "clang/AST/EvaluatedExprVisitor.h"
#include "clang/AST/StmtVisitor.h"
#include "clang/Basic/SourceManager.h"

namespace autocheck {

#define MAX_STACK_DEPTH 10

static unsigned int StackDepth = 0;

static bool isExprWithAutosarSideEffects(const clang::Expr *E) {
  E = E->IgnoreParenImpCasts();
  if (const clang::UnaryOperator *DerefOp =
          llvm::dyn_cast_if_present<clang::UnaryOperator>(E))
    if (DerefOp->getOpcode() == clang::UO_Deref)
      return true;
  // Since E is subexpression from unary expression ++, -- or it's an
  // expression on the left hand side of assignment operators, we consider that
  // it has side effects if it's variable with pointer or reference type or if
  // it isn't local.
  if (const clang::DeclRefExpr *DRF =
          llvm::dyn_cast_if_present<clang::DeclRefExpr>(E)) {
    const clang::ValueDecl *VD = DRF->getDecl();
    const clang::Type *T = VD->getType().getTypePtr();
    if (T->isReferenceType() || T->isPointerType())
      return true;
    if (const clang::VarDecl *VarD =
            llvm::dyn_cast_if_present<clang::VarDecl>(VD))
      if (!VarD->isLocalVarDeclOrParm())
        return true;
  }
  return false;
}

static bool
hasFunctionAutosarSideEffectsInDefinition(const clang::Stmt *Body,
                                          const clang::ASTContext &Ctx,
                                          bool IncludePossibleEffects) {
  if (!Body)
    return false;

  StackDepth++;

  if (StackDepth > MAX_STACK_DEPTH)
    return false;

  // Go through all expressions recursively in function definition and look for
  // expressions with side effects.
  for (auto ChildNode : Body->children()) {
    if (hasFunctionAutosarSideEffectsInDefinition(ChildNode, Ctx,
                                                  IncludePossibleEffects)) {
      StackDepth--;
      return true;
    }
    if (const clang::Expr *E =
            llvm::dyn_cast_if_present<clang::Expr>(ChildNode)) {
      E = E->IgnoreParenImpCasts();
      if (!E) {
        StackDepth--;
        return false;
      }
      // Check subexpression of unary operators ++ and --.
      if (const clang::UnaryOperator *UO =
              llvm::dyn_cast_if_present<clang::UnaryOperator>(E)) {
        if (UO->isIncrementOp() || UO->isDecrementOp()) {
          const clang::Expr *SubExpr = UO->getSubExpr();
          if (isExprWithAutosarSideEffects(SubExpr)) {
            StackDepth--;
            return true;
          }
          continue;
        }
      }

      // Check left hand side of assignment operators.
      if (const clang::BinaryOperator *BO =
              llvm::dyn_cast_if_present<clang::BinaryOperator>(E)) {
        if (BO->isAssignmentOp()) {
          const clang::Expr *LHS = BO->getLHS();
          if (isExprWithAutosarSideEffects(LHS)) {
            StackDepth--;
            return true;
          }
          continue;
        }
      }

      if (hasAutosarSideEffects(E, Ctx, IncludePossibleEffects)) {
        StackDepth--;
        return true;
      }
    }
  }
  StackDepth--;
  return false;
}

static bool hasDeclAutosarSideEffects(const clang::Decl *D,
                                      const clang::ASTContext &Ctx,
                                      bool IncludePossibleEffects) {
  const clang::FunctionDecl *FD =
      llvm::dyn_cast_if_present<clang::FunctionDecl>(D);
  if (!FD)
    return false;
  // If function is from standard library we consider it has side
  // effects unless it has attributes ConstAttr or PureAttr.
  if (Ctx.getSourceManager().isInSystemHeader(FD->getBeginLoc())) {
    bool IsPure =
        FD->hasAttr<clang::ConstAttr>() || FD->hasAttr<clang::PureAttr>();
    return IsPure || !IncludePossibleEffects;
  }

  FD = FD->getDefinition();

  if (!FD)
    return false;

  const clang::Stmt *Body = FD->getBody();
  if (!Body)
    return false;

  return hasFunctionAutosarSideEffectsInDefinition(Body, Ctx,
                                                   IncludePossibleEffects);
}

/// Look for any side effects within a Stmt.
///
/// This implementation was taken from Expr but modified to call our custom
/// implementation of hasSideEffect. Any updates to the original should be
/// mirrored here.
class SideEffectFinder
    : public clang::ConstEvaluatedExprVisitor<SideEffectFinder> {
  typedef ConstEvaluatedExprVisitor<SideEffectFinder> Inherited;
  const bool IncludePossibleEffects;
  bool HasSideEffects;

public:
  explicit SideEffectFinder(const clang::ASTContext &Context,
                            bool IncludePossible)
      : Inherited(Context), IncludePossibleEffects(IncludePossible),
        HasSideEffects(false) {}

  bool hasSideEffects() const { return HasSideEffects; }

  void VisitDecl(const clang::Decl *D) {
    if (!D)
      return;

    // We assume the caller checks subexpressions (eg, the initializer, VLA
    // bounds) for side-effects on our behalf.
    if (auto *VD = llvm::dyn_cast<clang::VarDecl>(D)) {
      // Registering a destructor is a side-effect.
      if (IncludePossibleEffects && VD->isThisDeclarationADefinition() &&
          VD->needsDestruction(Context))
        HasSideEffects = true;
    }
  }

  void VisitDeclStmt(const clang::DeclStmt *DS) {
    for (auto *D : DS->decls())
      VisitDecl(D);
    Inherited::VisitDeclStmt(DS);
  }

  void VisitExpr(const clang::Expr *E) {
    if (!HasSideEffects &&
        autocheck::hasAutosarSideEffects(E, Context, IncludePossibleEffects))
      HasSideEffects = true;
  }
};

/// Checks if the given expression has a side effect.
///
/// This implementation was taken from Expr and any updates to the original
/// should be mirrored here. The only difference is in handling function calls
/// for which we use a different approach.
bool hasAutosarSideEffects(const clang::Expr *E, const clang::ASTContext &Ctx,
                           bool IncludePossibleEffects) {
  // In circumstances where we care about definite side effects instead of
  // potential side effects, we want to ignore expressions that are part of a
  // macro expansion as a potential side effect.
  if (!IncludePossibleEffects && E->getExprLoc().isMacroID())
    return false;

  switch (E->getStmtClass()) {
  case clang::Stmt::NoStmtClass:
#define ABSTRACT_STMT(Type)
#define STMT(Type, Base) case clang::Stmt::Type##Class:
#define EXPR(Type, Base)
#include "clang/AST/StmtNodes.inc"
    llvm_unreachable("unexpected Expr kind");

  case clang::Stmt::DependentScopeDeclRefExprClass:
  case clang::Stmt::CXXUnresolvedConstructExprClass:
  case clang::Stmt::CXXDependentScopeMemberExprClass:
  case clang::Stmt::UnresolvedLookupExprClass:
  case clang::Stmt::UnresolvedMemberExprClass:
  case clang::Stmt::PackExpansionExprClass:
  case clang::Stmt::SubstNonTypeTemplateParmPackExprClass:
  case clang::Stmt::FunctionParmPackExprClass:
  case clang::Stmt::TypoExprClass:
  case clang::Stmt::RecoveryExprClass:
  case clang::Stmt::CXXFoldExprClass:
    // Make a conservative assumption for dependent nodes.
    return IncludePossibleEffects;

  case clang::Stmt::DeclRefExprClass:
  case clang::Stmt::ObjCIvarRefExprClass:
  case clang::Stmt::PredefinedExprClass:
  case clang::Stmt::IntegerLiteralClass:
  case clang::Stmt::FixedPointLiteralClass:
  case clang::Stmt::FloatingLiteralClass:
  case clang::Stmt::ImaginaryLiteralClass:
  case clang::Stmt::StringLiteralClass:
  case clang::Stmt::CharacterLiteralClass:
  case clang::Stmt::OffsetOfExprClass:
  case clang::Stmt::ImplicitValueInitExprClass:
  case clang::Stmt::UnaryExprOrTypeTraitExprClass:
  case clang::Stmt::AddrLabelExprClass:
  case clang::Stmt::GNUNullExprClass:
  case clang::Stmt::ArrayInitIndexExprClass:
  case clang::Stmt::NoInitExprClass:
  case clang::Stmt::CXXBoolLiteralExprClass:
  case clang::Stmt::CXXNullPtrLiteralExprClass:
  case clang::Stmt::CXXThisExprClass:
  case clang::Stmt::CXXScalarValueInitExprClass:
  case clang::Stmt::TypeTraitExprClass:
  case clang::Stmt::ArrayTypeTraitExprClass:
  case clang::Stmt::ExpressionTraitExprClass:
  case clang::Stmt::CXXNoexceptExprClass:
  case clang::Stmt::SizeOfPackExprClass:
  case clang::Stmt::ObjCStringLiteralClass:
  case clang::Stmt::ObjCEncodeExprClass:
  case clang::Stmt::ObjCBoolLiteralExprClass:
  case clang::Stmt::ObjCAvailabilityCheckExprClass:
  case clang::Stmt::CXXUuidofExprClass:
  case clang::Stmt::OpaqueValueExprClass:
  case clang::Stmt::SourceLocExprClass:
  case clang::Stmt::ConceptSpecializationExprClass:
  case clang::Stmt::RequiresExprClass:
  case clang::Stmt::SYCLUniqueStableNameExprClass:
    // These never have a side-effect.
    return false;

  case clang::Stmt::ConstantExprClass:
    // FIXME: Move this into the "return false;" block above.
    return hasAutosarSideEffects(
        llvm::cast<clang::ConstantExpr>(E)->getSubExpr(), Ctx,
        IncludePossibleEffects);

  case clang::Stmt::CallExprClass:
  case clang::Stmt::CXXOperatorCallExprClass:
  case clang::Stmt::CXXMemberCallExprClass:
  case clang::Stmt::CUDAKernelCallExprClass:
  case clang::Stmt::UserDefinedLiteralClass: {
    const clang::Decl *FD = llvm::cast<clang::CallExpr>(E)->getCalleeDecl();

    if (IncludePossibleEffects)
      return hasDeclAutosarSideEffects(FD, Ctx, IncludePossibleEffects);

    // We don't know a call definitely has side effects, except for calls
    // to pure/const functions that definitely don't.
    // If the call itself is considered side-effect free, check the operands.
    bool IsPure = FD && (FD->hasAttr<clang::ConstAttr>() ||
                         FD->hasAttr<clang::PureAttr>());
    if (IsPure || !IncludePossibleEffects)
      break;
    return true;
  }

  case clang::Stmt::BlockExprClass:
  case clang::Stmt::CXXBindTemporaryExprClass:
    if (!IncludePossibleEffects)
      break;
    return true;

  case clang::Stmt::MSPropertyRefExprClass:
  case clang::Stmt::MSPropertySubscriptExprClass:
  case clang::Stmt::CompoundAssignOperatorClass:
  case clang::Stmt::VAArgExprClass:
  case clang::Stmt::AtomicExprClass:
  case clang::Stmt::CXXThrowExprClass:
  case clang::Stmt::CXXNewExprClass:
  case clang::Stmt::CXXDeleteExprClass:
  case clang::Stmt::CoawaitExprClass:
  case clang::Stmt::DependentCoawaitExprClass:
  case clang::Stmt::CoyieldExprClass:
    // These always have a side-effect.
    return true;

  case clang::Stmt::StmtExprClass: {
    // StmtExprs have a side-effect if any substatement does.
    SideEffectFinder Finder(Ctx, IncludePossibleEffects);
    Finder.Visit(llvm::cast<clang::StmtExpr>(E)->getSubStmt());
    return Finder.hasSideEffects();
  }

  case clang::Stmt::ExprWithCleanupsClass:
    if (IncludePossibleEffects)
      if (llvm::cast<clang::ExprWithCleanups>(E)->cleanupsHaveSideEffects())
        return true;
    break;

  case clang::Stmt::ParenExprClass:
  case clang::Stmt::ArraySubscriptExprClass:
  case clang::Stmt::MatrixSubscriptExprClass:
  case clang::Stmt::OMPArraySectionExprClass:
  case clang::Stmt::OMPArrayShapingExprClass:
  case clang::Stmt::OMPIteratorExprClass:
  case clang::Stmt::MemberExprClass:
  case clang::Stmt::ConditionalOperatorClass:
  case clang::Stmt::BinaryConditionalOperatorClass:
  case clang::Stmt::CompoundLiteralExprClass:
  case clang::Stmt::ExtVectorElementExprClass:
  case clang::Stmt::DesignatedInitExprClass:
  case clang::Stmt::DesignatedInitUpdateExprClass:
  case clang::Stmt::ArrayInitLoopExprClass:
  case clang::Stmt::ParenListExprClass:
  case clang::Stmt::CXXPseudoDestructorExprClass:
  case clang::Stmt::CXXRewrittenBinaryOperatorClass:
  case clang::Stmt::CXXStdInitializerListExprClass:
  case clang::Stmt::SubstNonTypeTemplateParmExprClass:
  case clang::Stmt::MaterializeTemporaryExprClass:
  case clang::Stmt::ShuffleVectorExprClass:
  case clang::Stmt::ConvertVectorExprClass:
  case clang::Stmt::AsTypeExprClass:
    // These have a side-effect if any subexpression does.
    break;

  case clang::Stmt::UnaryOperatorClass:
    if (llvm::cast<clang::UnaryOperator>(E)->isIncrementDecrementOp())
      return true;
    break;

  case clang::Stmt::BinaryOperatorClass:
    if (llvm::cast<clang::BinaryOperator>(E)->isAssignmentOp())
      return true;
    break;

  case clang::Stmt::InitListExprClass:
    // FIXME: The children for an InitListExpr doesn't include the array filler.
    if (const clang::Expr *E =
            llvm::cast<clang::InitListExpr>(E)->getArrayFiller())
      if (hasAutosarSideEffects(E, Ctx, IncludePossibleEffects))
        return true;
    break;

  case clang::Stmt::GenericSelectionExprClass:
    return hasAutosarSideEffects(
        llvm::cast<clang::GenericSelectionExpr>(E)->getResultExpr(), Ctx,
        IncludePossibleEffects);

  case clang::Stmt::ChooseExprClass:
    return hasAutosarSideEffects(
        llvm::cast<clang::ChooseExpr>(E)->getChosenSubExpr(), Ctx,
        IncludePossibleEffects);

  case clang::Stmt::CXXDefaultArgExprClass:
    return hasAutosarSideEffects(
        llvm::cast<clang::CXXDefaultArgExpr>(E)->getExpr(), Ctx,
        IncludePossibleEffects);

  case clang::Stmt::CXXDefaultInitExprClass: {
    const clang::FieldDecl *FD =
        llvm::cast<clang::CXXDefaultInitExpr>(E)->getField();
    if (const clang::Expr *E = FD->getInClassInitializer())
      return hasAutosarSideEffects(E, Ctx, IncludePossibleEffects);
    // If we've not yet parsed the initializer, assume it has side-effects.
    return true;
  }

  case clang::Stmt::CXXDynamicCastExprClass: {
    // A dynamic_cast expression has side-effects if it can throw.
    const clang::CXXDynamicCastExpr *DCE =
        llvm::cast<clang::CXXDynamicCastExpr>(E);
    if (DCE->getTypeAsWritten()->isReferenceType() &&
        DCE->getCastKind() == clang::CK_Dynamic)
      return true;
  }
    [[fallthrough]];
  case clang::Stmt::ImplicitCastExprClass:
  case clang::Stmt::CStyleCastExprClass:
  case clang::Stmt::CXXStaticCastExprClass:
  case clang::Stmt::CXXReinterpretCastExprClass:
  case clang::Stmt::CXXConstCastExprClass:
  case clang::Stmt::CXXAddrspaceCastExprClass:
  case clang::Stmt::CXXFunctionalCastExprClass:
  case clang::Stmt::BuiltinBitCastExprClass: {
    // While volatile reads are side-effecting in both C and C++, we treat them
    // as having possible (not definite) side-effects. This allows idiomatic
    // code to behave without warning, such as sizeof(*v) for a volatile-
    // qualified pointer.
    if (!IncludePossibleEffects)
      break;

    const clang::CastExpr *CE = llvm::cast<clang::CastExpr>(E);
    if (CE->getCastKind() == clang::CK_LValueToRValue &&
        CE->getSubExpr()->getType().isVolatileQualified())
      return true;
    break;
  }

  case clang::Stmt::CXXTypeidExprClass:
    // typeid might throw if its subexpression is potentially-evaluated, so has
    // side-effects in that case whether or not its subexpression does.
    return llvm::cast<clang::CXXTypeidExpr>(E)->isPotentiallyEvaluated();

  case clang::Stmt::CXXConstructExprClass:
  case clang::Stmt::CXXTemporaryObjectExprClass: {
    const clang::CXXConstructExpr *CE = llvm::cast<clang::CXXConstructExpr>(E);
    if (!CE->getConstructor()->isTrivial() && IncludePossibleEffects)
      return true;
    // A trivial constructor does not add any side-effects of its own. Just look
    // at its arguments.
    break;
  }

  case clang::Stmt::CXXInheritedCtorInitExprClass: {
    const auto *ICIE = llvm::cast<clang::CXXInheritedCtorInitExpr>(E);
    if (!ICIE->getConstructor()->isTrivial() && IncludePossibleEffects)
      return true;
    break;
  }

  case clang::Stmt::LambdaExprClass: {
    const clang::LambdaExpr *LE = llvm::cast<clang::LambdaExpr>(E);
    for (clang::Expr *E : LE->capture_inits())
      if (E && hasAutosarSideEffects(E, Ctx, IncludePossibleEffects))
        return true;
    return false;
  }

  case clang::Stmt::PseudoObjectExprClass: {
    // Only look for side-effects in the semantic form, and look past
    // OpaqueValueExpr bindings in that form.
    const clang::PseudoObjectExpr *PO = llvm::cast<clang::PseudoObjectExpr>(E);
    for (clang::PseudoObjectExpr::const_semantics_iterator
             I = PO->semantics_begin(),
             E = PO->semantics_end();
         I != E; ++I) {
      const clang::Expr *Subexpr = *I;
      if (const clang::OpaqueValueExpr *OVE =
              llvm::dyn_cast<clang::OpaqueValueExpr>(Subexpr))
        Subexpr = OVE->getSourceExpr();
      if (hasAutosarSideEffects(Subexpr, Ctx, IncludePossibleEffects))
        return true;
    }
    return false;
  }

  case clang::Stmt::ObjCBoxedExprClass:
  case clang::Stmt::ObjCArrayLiteralClass:
  case clang::Stmt::ObjCDictionaryLiteralClass:
  case clang::Stmt::ObjCSelectorExprClass:
  case clang::Stmt::ObjCProtocolExprClass:
  case clang::Stmt::ObjCIsaExprClass:
  case clang::Stmt::ObjCIndirectCopyRestoreExprClass:
  case clang::Stmt::ObjCSubscriptRefExprClass:
  case clang::Stmt::ObjCBridgedCastExprClass:
  case clang::Stmt::ObjCMessageExprClass:
  case clang::Stmt::ObjCPropertyRefExprClass:
    // FIXME: Classify these cases better.
    if (IncludePossibleEffects)
      return true;
    break;
  }

  // Recurse to children.
  for (const clang::Stmt *SubStmt : E->children())
    if (SubStmt && hasAutosarSideEffects(llvm::cast<clang::Expr>(SubStmt), Ctx,
                                         IncludePossibleEffects))
      return true;

  return false;
}

} // namespace autocheck
