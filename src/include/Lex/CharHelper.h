//===--- CharHelper.h - Helper functions for character constants ----------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares helper functions for character constants.
//
//===----------------------------------------------------------------------===//

#ifndef LEX_CHAR_HELPER_H
#define LEX_CHAR_HELPER_H

namespace autocheck {

bool isASCII(char Char);

bool isCPlusPlusStandardChar(char Char);

bool isValidCommentChar(char Char);

bool isValidEscapeChar(char Char);

bool isHexDigit(char Char);

bool isOctalDigit(char Char);

} // namespace autocheck

#endif
