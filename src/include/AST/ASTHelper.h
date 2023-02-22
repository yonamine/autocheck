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

#include "clang/AST/Decl.h"

namespace autocheck {

bool isSwapFunction(const clang::FunctionDecl *FD);

}

#endif
