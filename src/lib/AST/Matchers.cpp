//===--- Matchers.cpp - Matchers for AUTOSAR rule checks ------------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements AST Matchers that check AUTOSAR rules.
//
//===----------------------------------------------------------------------===//

#include "AST/Matchers.h"

#include "Diagnostics/AutocheckDiagnostic.h"

namespace autocheck {

using namespace clang::ast_matchers;

/* Implementation of UnusedReturnMatcher */

UnusedReturnMatcher::Callback::Callback(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

void UnusedReturnMatcher::Callback::run(
    const MatchFinder::MatchResult &Result) {
  const BoundNodes &BN = Result.Nodes;
  if (const clang::CallExpr *CE = BN.getNodeAs<clang::CallExpr>("unusedReturn"))
    AutocheckDiagnostic::reportWarning(DE, CE->getBeginLoc(),
                                       AutocheckWarnings::unusedReturnValue);
}

StatementMatcher UnusedReturnMatcher::makeMatcher() {
  std::string CallExprName("CallExpr");

  auto hasReturnValue =
      hasDeclaration(functionDecl(returns(unless(voidType()))));

  auto callExprNotInCondition = unless(
      hasCondition(anyOf(hasDescendant(callExpr(equalsBoundNode(CallExprName))),
                         callExpr(equalsBoundNode(CallExprName)))));

  // Match if node has ancestor which is statement other than the ones that are
  // listed. This is necessary because callExpr of a called function could be
  // descendant of these statements and still be considered unused.
  auto usedInsideStatement = hasAncestor(stmt(unless(anyOf(
      exprWithCleanups(), materializeTemporaryExpr(), lambdaExpr(),
      implicitCastExpr(), autoreleasePoolStmt(), cxxBindTemporaryExpr(),
      cxxDependentScopeMemberExpr(), cxxForRangeStmt(), unresolvedMemberExpr(),
      unresolvedLookupExpr(), memberExpr(), unaryExprOrTypeTraitExpr(),
      parenExpr(), compoundStmt(), cxxTryStmt(), caseStmt(), defaultStmt(),
      explicitCastExpr(unless(hasDestinationType(voidType()))),
      forStmt(callExprNotInCondition), ifStmt(callExprNotInCondition),
      whileStmt(callExprNotInCondition), switchStmt(callExprNotInCondition),
      doStmt(callExprNotInCondition)))));

  // Matches against callExpr which:
  // (i) has return value other than void,
  // (ii) is considered unused, and
  // (iii) is not predefined operator call.
  // callExpr is considered unused if it is not part of a statement, or it is a
  // part of a statement listed in usedInsideStatement matcher.
  return callExpr(callExpr().bind(CallExprName),
                  allOf(hasReturnValue, unless(usedInsideStatement)),
                  unless(callExpr(cxxOperatorCallExpr())))
      .bind("unusedReturn");
}

bool UnusedReturnMatcher::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::unusedReturnValue);
}

/* Implementation of SelfAssignmentMatcher */

SelfAssignmentMatcher::Callback::Callback(clang::DiagnosticsEngine &DE)
    : DE(DE) {}

void SelfAssignmentMatcher::Callback::run(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
      "copyOrMoveAssignmentOperator");
  AutocheckDiagnostic::reportWarning(
      DE, MatchedDecl->getLocation(),
      AutocheckWarnings::selfAssignmentUnhandled);
}

DeclarationMatcher SelfAssignmentMatcher::makeMatcher() {
  // We don't care about deleted, default or implicit operator implementations.
  const auto IsUserDefined = cxxMethodDecl(
      isDefinition(), unless(anyOf(isDeleted(), isImplicit(), isDefaulted())));

  // We don't need to worry when a copy or move assignment operator gets the
  // other object by value.
  const auto HasReferenceParam =
      cxxMethodDecl(hasParameter(0, parmVarDecl(hasType(referenceType()))));

  // Self-check: Code compares something with 'this' pointer. We don't check
  // whether it is actually the parameter what we compare.
  const auto HasNoSelfCheck = cxxMethodDecl(unless(hasDescendant(
      binaryOperation(hasAnyOperatorName("==", "!="),
                      hasEitherOperand(ignoringParenCasts(cxxThisExpr()))))));

  // Both copy-and-swap and copy-and-move method creates a copy first and
  // assign it to 'this' with swap or move.
  // In the non-template case, we can search for the copy constructor call.
  const auto HasNonTemplateSelfCopy = cxxMethodDecl(
      ofClass(cxxRecordDecl(unless(hasAncestor(classTemplateDecl())))),
      traverse(clang::TK_AsIs,
               hasDescendant(cxxConstructExpr(hasDeclaration(cxxConstructorDecl(
                   isCopyConstructor(), ofClass(equalsBoundNode("class"))))))));

  // In the template case, we need to handle two separate cases: 1) a local
  // variable is created with the copy, 2) copy is created only as a temporary
  // object.
  const auto HasTemplateSelfCopy = cxxMethodDecl(
      ofClass(cxxRecordDecl(hasAncestor(classTemplateDecl()))),
      anyOf(hasDescendant(
                varDecl(hasType(cxxRecordDecl(equalsBoundNode("class"))),
                        hasDescendant(parenListExpr()))),
            hasDescendant(cxxUnresolvedConstructExpr(hasDescendant(declRefExpr(
                hasType(cxxRecordDecl(equalsBoundNode("class")))))))));

  // If inside the copy or move assignment operator another assignment operator
  // is called on 'this' we assume that self-check might be handled inside this
  // nested operator.
  const auto HasNoNestedSelfAssign =
      cxxMethodDecl(unless(hasDescendant(cxxMemberCallExpr(callee(cxxMethodDecl(
          hasName("operator="), ofClass(equalsBoundNode("class"))))))));

  DeclarationMatcher AdditionalMatcher = cxxMethodDecl();

  // Matcher for standard smart pointers.
  const auto SmartPointerType = qualType(hasUnqualifiedDesugaredType(
      recordType(hasDeclaration(classTemplateSpecializationDecl(
          hasAnyName("::std::shared_ptr", "::std::unique_ptr",
                     "::std::weak_ptr", "::std::auto_ptr"),
          templateArgumentCountIs(1))))));

  // We will warn only if the class has a pointer or a C array field which
  // probably causes a problem during self-assignment (e.g. first resetting
  // the pointer member, then trying to access the object pointed by the
  // pointer, or memcpy overlapping arrays).
  AdditionalMatcher = cxxMethodDecl(ofClass(cxxRecordDecl(
      has(fieldDecl(anyOf(hasType(pointerType()), hasType(SmartPointerType),
                          hasType(arrayType())))))));

  return cxxMethodDecl(
             ofClass(cxxRecordDecl().bind("class")),
             anyOf(isCopyAssignmentOperator(), isMoveAssignmentOperator()),
             IsUserDefined, HasReferenceParam, HasNoSelfCheck,
             unless(HasNonTemplateSelfCopy), unless(HasTemplateSelfCopy),
             HasNoNestedSelfAssign, AdditionalMatcher)
      .bind("copyOrMoveAssignmentOperator");
}

bool SelfAssignmentMatcher::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::selfAssignmentUnhandled);
}

} // namespace autocheck
