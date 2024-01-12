//===--- AutocheckLex.h - Checker for lexer rules -------------------------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file performs checks for rules related to the lexer.
//
// This implements the following checks:
// - [A2-3-1]  Only those characters specified in the C++ Language Standard
//             basic source character set shall be used in the source code.
// - [A2-5-1]  Trigraphs shall not be used.
// - [A2-5-2]  Digraphs shall not be used.
// - [A2-7-1]  The character \ shall not occur as a last character of a C++
//             comment.
// - [M2-7-1]  The character sequence /* shall not be used within a C-style
//             comment.
// - [A2-11-1] Volatile keyword shall not be used.
// - [A2-13-1] Only those escape sequences that are defined in ISO/IEC
//             14882:2014 shall be used.
// - [M2-13-2] Octal constants (other than zero) and octal escape sequences
//             (other than “\0” ) shall not be used.
// - [M2-13-4] Literal suffixes shall be upper case.
// - [A2-13-5] Hexadecimal constants should be upper case.
// - [A2-13-6] Universal character names shall be used only inside character or
//             string literals.
// - [A5-2-1]  dynamic_cast should not be used.
// - [A5-2-4]  reinterpret_cast shall not be used.
// - [A6-6-1]  The goto statement shall not be used.
// - [A13-6-1] Digit sequences separators ' shall only be used as follows: (1)
//             for decimal, every 3 digits, (2) for hexadecimal, every 2 digits,
//             (3) for binary, every 4 digits.
// - [M16-0-5] Arguments to a function-like macro shall not contain tokens that
//             look like pre-processing directives.
// - [M16-0-8] If the # token appears as the first token on a line, then it
//             shall be immediately followed by a pre-processing token.
// - [A16-2-1] The ', ", /*, //, \ characters shall not occur in a header file
//             name or in #include directive.
// - [A16-6-1] #error directive shall not be used.
//
//===----------------------------------------------------------------------===//

#ifndef LEX_AUTOCHECK_LEX_H
#define LEX_AUTOCHECK_LEX_H

#include "Diagnostics/AutocheckDiagnostic.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Token.h"

namespace autocheck {

namespace lex {

/// Check token for AUTOSAR rule violations.
void CheckToken(AutocheckDiagnostic &AD, clang::CompilerInstance &CI,
                const clang::Token &Tok);

/// Check file associated with the given FileID for AUTOSAR rule violations that
/// must be checked before any file preprocessing is done.
void RunRawLexer(AutocheckDiagnostic &AD, const clang::CompilerInstance &CI,
                 clang::FileID FID);

} // namespace lex

} // namespace autocheck

#endif
