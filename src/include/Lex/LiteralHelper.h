//===--- LiteralHelper.h - Helper functions for literals ------------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares helper functions for literals.
//
//===----------------------------------------------------------------------===//

#ifndef LEX_LITERAL_HELPER_H
#define LEX_LITERAL_HELPER_H

#include "clang/Basic/TokenKinds.h"
#include "llvm/ADT/StringRef.h"

namespace autocheck {

// Data class that holds parsed segments and information about a numeric
// literal.
struct ParsedNumber {
  const char *WholeBegin = nullptr;
  unsigned WholeLen = 0;

  const char *FractionBegin = nullptr;
  unsigned FractionLen = 0;

  const char *ExponentBegin = nullptr;
  unsigned ExponentLen = 0;

  const char *SuffixBegin = nullptr;
  unsigned SuffixLen = 0;

  const char *RadixPos = nullptr;

  unsigned Base = 0;

  bool HasSeparator = false;
  bool ExponentHasSeparator = false;
};

ParsedNumber parseNumberLiteral(const llvm::StringRef &Literal);

bool checkDigitSeparators(const ParsedNumber &Number);

bool isCharacterLiteral(clang::tok::TokenKind Kind);

} // namespace autocheck

#endif
