//===--- ASTHelper.h - Helper functions for AST nodes ---------------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares helper functions for AST nodes.
//
//===----------------------------------------------------------------------===//

#ifndef AST_AST_HELPER_H
#define AST_AST_HELPER_H

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Type.h"

namespace autocheck {

bool isSwapFunction(const clang::FunctionDecl *FD);

/// Compares if passed in types T1 and T2 are exactly the same (e.g. int8_t is
/// not the same as char even if it is typedef-ed like that).
///
/// This operation should normally be performed simply like T1 == T2, but this
/// does not work for types declared with a 'using' declaration because each
/// instance will have a different pointer.
bool areTypesEquivalent(clang::ASTContext &AC, clang::QualType T1,
                        clang::QualType T2);

} // namespace autocheck

#endif
