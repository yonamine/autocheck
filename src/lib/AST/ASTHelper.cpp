//===--- ASTHelper.cpp - Helper functions for AST nodes -------------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements helper functions for AST nodes.
//
//===----------------------------------------------------------------------===//

#include "AST/ASTHelper.h"

namespace autocheck {

bool isSwapFunction(const clang::FunctionDecl *FD) {
  // Return false if function name doesn't contain some variation of word
  // 'swap' in it.
  std::string FunctionName = FD->getNameInfo().getName().getAsString();
  std::transform(FunctionName.begin(), FunctionName.end(), FunctionName.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  if (FunctionName.find("swap") == std::string::npos) {
    return false;
  }

  if (FD->param_size() != 2)
    return false;

  clang::QualType FirstArgumentType = FD->getParamDecl(0)->getType();
  clang::QualType SecondArgumentType = FD->getParamDecl(1)->getType();

  // QualTypes should be the same in swap functions.
  if (FirstArgumentType != SecondArgumentType)
    return false;

  // Arguments in swap functions must be passed by reference or pointer.
  if (!FirstArgumentType->isLValueReferenceType() &&
      !FirstArgumentType->isPointerType())
    return false;

  if (FirstArgumentType->isPointerType())
    FirstArgumentType = FirstArgumentType->getPointeeType();
  else
    FirstArgumentType = FirstArgumentType.getNonReferenceType();

  // If arguments are passed by const reference or pointer, this function
  // doesn't actually swap them.
  if (FirstArgumentType.isConstQualified())
    return false;

  return true;
}

} // namespace autocheck
