#ifndef AST_SIDE_EFFECT_CHECKER_H
#define AST_SIDE_EFFECT_CHECKER_H

#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"

namespace autocheck {

bool hasAutosarSideEffects(const clang::Expr *E, const clang::ASTContext &Ctx,
                           bool IncludePossibleEffects = true);

} // namespace autocheck

#endif
