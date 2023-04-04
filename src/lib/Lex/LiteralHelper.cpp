//===--- LiteralHelper.cpp - Helper functions for literals ----------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements helper functions for literals.
//
//===----------------------------------------------------------------------===//

#include "Lex/LiteralHelper.h"

#include "Lex/CharHelper.h"
#include "clang/Basic/TokenKinds.h"
#include "llvm/Support/Debug.h"

#define HEX_SEPARATOR_DISTANCE 3
#define DEC_SEPARATOR_DISTANCE 4
#define BIN_SEPARATOR_DISTANCE 5

namespace autocheck {

// Helper function for parseNumberLiteral. This should not be used directly.
static void parseNumberLiteral_(llvm::StringRef Literal, ParsedNumber &Result,
                                std::function<bool(char)> IsDigit,
                                std::function<bool(char)> IsExponentSymbol) {
  Result.WholeBegin = Literal.begin();
  for (const char *CurrentChar = Literal.begin(); CurrentChar != Literal.end();
       CurrentChar++) {
    if (*CurrentChar == '.') {
      Result.RadixPos = CurrentChar;
      Result.FractionBegin = CurrentChar + 1;
      continue;
    } else if (IsExponentSymbol(*CurrentChar)) {
      if (Result.RadixPos == nullptr) {
        Result.RadixPos = CurrentChar;
      }
      Result.ExponentBegin = CurrentChar + 1;
      continue;
    } else if (*CurrentChar == '\'') {
      if (Result.ExponentBegin == nullptr)
        Result.HasSeparator = true;
      else
        Result.ExponentHasSeparator = true;
    } else if (!IsDigit(*CurrentChar) &&
               *CurrentChar != '+' && *CurrentChar != '-') {
      if (Result.RadixPos == nullptr) {
        Result.RadixPos = CurrentChar;
      }
      if (Result.ExponentBegin == nullptr) {
        Result.ExponentBegin = CurrentChar;
      }
      Result.SuffixBegin = CurrentChar;
      Result.SuffixLen = Literal.end() - CurrentChar;
      break;
    }
    if (Result.RadixPos == nullptr) {
      Result.WholeLen++;
    } else if (Result.ExponentBegin == nullptr) {
      Result.FractionLen++;
    } else {
      Result.ExponentLen++;
    }
  }
  if (Result.RadixPos == nullptr) {
    Result.RadixPos = Literal.end();
    Result.FractionBegin = Literal.end();
  }
  if (Result.ExponentBegin == nullptr) {
    Result.ExponentBegin = Literal.end();
  }
  if (Result.SuffixBegin == nullptr) {
    Result.SuffixBegin = Literal.end();
  }
}

// Helper function for parsing any type of numeric literal.
ParsedNumber parseNumberLiteral(const llvm::StringRef &Literal) {
  ParsedNumber Result;
  if (Literal.startswith_insensitive("0x")) { // Hexadecimal
    Result.Base = 16;
    parseNumberLiteral_(Literal.slice(2, Literal.size()), Result, isHexDigit,
                        [](char C) { return C == 'p' || C == 'P'; });
  } else if (Literal.starts_with_insensitive("0b")) { // Binary
    Result.Base = 2;
    parseNumberLiteral_(
        Literal.slice(2, Literal.size()), Result,
        [](char C) { return C == '0' || C == '1'; },
        [](char C) { return false; });
  } else if (Literal[0] == '0' &&
             Literal.find('.') == llvm::StringRef::npos) { // Octal
    Result.Base = 8;
    parseNumberLiteral_(Literal, Result, isOctalDigit,
                        [](char C) { return false; });
  } else { // Decimal
    Result.Base = 10;
    parseNumberLiteral_(
        Literal, Result, [](char C) { return C >= '0' && C <= '9'; },
        [](char C) { return C == 'e' || C == 'E'; });
  }
  return Result;
}

bool checkDigitSeparators(const ParsedNumber &Number) {
  unsigned SeparatorDistance;
  switch (Number.Base) {
  case 2:
    SeparatorDistance = BIN_SEPARATOR_DISTANCE;
    break;
  case 8:
    return !Number.HasSeparator;
  case 10:
    SeparatorDistance = DEC_SEPARATOR_DISTANCE;
    break;
  case 16:
    SeparatorDistance = HEX_SEPARATOR_DISTANCE;
    break;
  default:
    assert(false && "Unsupported base for number.");
    return false;
  }
  if (Number.HasSeparator) {
    for (unsigned offset = 1; offset <= Number.WholeLen; offset++) {
      if ((offset % SeparatorDistance == 0) !=
          (Number.WholeBegin[Number.WholeLen - offset] == '\'')) {
        return false;
      }
    }
    if (Number.Base == 2 || Number.Base == 8)
      return true;
    for (unsigned offset = 1; offset <= Number.FractionLen; offset++) {
      if ((offset % SeparatorDistance == 0) !=
          (Number.FractionBegin[offset - 1] == '\'')) {
        return false;
      }
    }
  }
  if (Number.ExponentHasSeparator) {
    for (unsigned offset = 1; offset <= Number.ExponentLen; offset++) {
      if ((offset % DEC_SEPARATOR_DISTANCE == 0) !=
          (Number.ExponentBegin[Number.ExponentLen - offset] == '\'')) {
        return false;
      }
    }
  }
  return true;
}

bool isCharacterLiteral(clang::tok::TokenKind Kind) {
  return Kind == clang::tok::char_constant ||
         Kind == clang::tok::wide_char_constant ||
         Kind == clang::tok::utf8_char_constant ||
         Kind == clang::tok::utf16_char_constant ||
         Kind == clang::tok::utf32_char_constant;
}

} // namespace autocheck
