//===--- AutocheckLex.h - Checker for lexer rules -------------------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file performs checks for rules related to the lexer.
//
//===----------------------------------------------------------------------===//

#include "Lex/AutocheckLex.h"

#include "Diagnostics/AutocheckDiagnostic.h"
#include "Lex/CharHelper.h"
#include "Lex/LiteralHelper.h"
#include "clang/Basic/IdentifierTable.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/ADT/StringRef.h"
#include <functional>

#define MAX_OCTAL_ESCAPE_LENGTH 3

namespace autocheck {

namespace lex {

static bool checkHexEscape(const llvm::StringRef &Literal) {
  return Literal.size() > 1 && isHexDigit(Literal[1]);
}

static bool checkUCNEscape(const llvm::StringRef &Literal,
                           unsigned CodeLength) {
  // Check if there are enough characters
  if (Literal.size() <= CodeLength + 1)
    return false;
  // All characters must be hexadecimal digits
  for (int i = 0; i < CodeLength; i++) {
    if (!isHexDigit(Literal[i + 1])) {
      return false;
    }
  }
  return true;
}

void CheckToken(const AutocheckContext &Context, clang::CompilerInstance &CI,
                const clang::Token &Tok) {
  clang::DiagnosticsEngine &DE = CI.getDiagnostics();
  const clang::SourceManager &SM = CI.getSourceManager();

  // Check should this location be checked for AUTOSAR rules based on the
  // currently set flags.
  const clang::SourceLocation Loc = Tok.getLocation();
  if (!appropriateHeaderLocation(DE, Loc))
    return;

  // [A2-13-5] Hexadecimal constants should be upper case.
  if (Context.isEnabled(AutocheckWarnings::hexConstUpperCase) &&
      Tok.is(clang::tok::numeric_constant)) {
    llvm::StringRef Literal(Tok.getLiteralData(), Tok.getLength());
    if (Literal.startswith_insensitive("0x")) {
      for (int offset = 2; offset < Tok.getLength(); offset++) {
        if (Literal[offset] >= 'a' && Literal[offset] <= 'f') {
          AutocheckDiagnostic::reportWarning(
              DE, Tok.getLocation().getLocWithOffset(offset),
              AutocheckWarnings::hexConstUpperCase);
          break;
        } else if (Literal[offset] == 'p' || Literal[offset] == 'P') {
          // Reached the exponent, don't check any more digits.
          break;
        }
      }
    }
  }

  // [A2-13-1] Only those escape sequences that are defined in
  // ISO/IEC 14882:2014 shall be used.
  if (Context.isEnabled(AutocheckWarnings::nonIsoEscapeSequence) &&
      (clang::tok::isStringLiteral(Tok.getKind()) ||
       isCharacterLiteral(Tok.getKind()))) {
    llvm::StringRef Literal(Tok.getLiteralData(), Tok.getLength());

    for (int offset = 0; offset < Tok.getLength() - 1; offset++) {
      char CurrentChar = Literal[offset];
      if (CurrentChar != '\\')
        continue;
      CurrentChar = Literal[offset + 1];
      if (isValidEscapeChar(CurrentChar)) {
        // Hexadecimal escape codes and universal character names require
        // additional checks.
        llvm::StringRef EscapeString(Literal.data() + offset + 1,
                                     Literal.size() - offset - 1);
        if (tolower(CurrentChar) == 'x') { // Hex escape
          if (!checkHexEscape(EscapeString))
            AutocheckDiagnostic::reportWarning(
                DE, Tok.getLocation().getLocWithOffset(offset),
                AutocheckWarnings::nonIsoEscapeSequence);
        } else if (CurrentChar == 'u') {
          // Universal character name \uXXXX
          if (!checkUCNEscape(EscapeString, 4))
            AutocheckDiagnostic::reportWarning(
                DE, Tok.getLocation().getLocWithOffset(offset),
                AutocheckWarnings::nonIsoEscapeSequence);

        } else if (CurrentChar == 'U') {
          // Universal character name \UXXXXXXXX
          if (!checkUCNEscape(EscapeString, 8))
            AutocheckDiagnostic::reportWarning(
                DE, Tok.getLocation().getLocWithOffset(offset),
                AutocheckWarnings::nonIsoEscapeSequence);
        }
      } else {
        AutocheckDiagnostic::reportWarning(
            DE, Tok.getLocation().getLocWithOffset(offset),
            AutocheckWarnings::nonIsoEscapeSequence);
      }
    }
  }

  // [M2-13-2] Octal constants (other than zero) and octal escape sequences
  // (other than "\0") shall not be used.
  if (Context.isEnabled(AutocheckWarnings::octalConstantUsed)) {
    // Case 1: Octal constants (other than zero) shall not be used.
    if (Tok.is(clang::tok::numeric_constant)) {
      llvm::StringRef Literal(Tok.getLiteralData(), Tok.getLength());
      if (Literal.size() > 1 && Literal[0] == '0' && isOctalDigit(Literal[1]) &&
          !Literal.contains('.') && !Literal.contains_insensitive('e')) {
        AutocheckDiagnostic::reportWarning(
            DE, Tok.getLocation(), AutocheckWarnings::octalConstantUsed);
      }
    }

    // Case 2: Octal escape sequences (other than "\0") shall not be used.
    if (clang::tok::isStringLiteral(Tok.getKind()) ||
        isCharacterLiteral(Tok.getKind())) {
      llvm::StringRef Literal(Tok.getLiteralData(), Tok.getLength());
      size_t Start = 0;
      while ((Start = Literal.find('\\', Start)) != llvm::StringRef::npos) {
        Start++; // Move after '\'.
        size_t EscapeCodeLength = 0;
        bool isZero = true;
        while (Start + EscapeCodeLength < Literal.size() &&
               EscapeCodeLength < MAX_OCTAL_ESCAPE_LENGTH &&
               isOctalDigit(Literal[Start + EscapeCodeLength])) {
          if (Literal[Start + EscapeCodeLength] != '0')
            isZero = false;
          EscapeCodeLength++;
        }
        if (!isZero) {
          AutocheckDiagnostic::reportWarning(
              DE, Tok.getLocation(), AutocheckWarnings::octalConstantUsed);
        }
        Start += EscapeCodeLength; // Move after the escape code.
      }
    }
  }

  // [M2-13-4] Literal suffixes shall be upper case.
  if (Context.isEnabled(AutocheckWarnings::literalSuffixLowerCase) &&
      Tok.is(clang::tok::numeric_constant)) {
    llvm::StringRef Literal(Tok.getLiteralData(), Tok.getLength());

    ParsedNumber ParsedLiteral = parseNumberLiteral(Literal);
    for (unsigned i = 0; i < ParsedLiteral.SuffixLen; ++i) {
      char C = ParsedLiteral.SuffixBegin[i];
      if (isalpha(C) && !isupper(C)) {
        AutocheckDiagnostic::reportWarning(
            DE, Tok.getLocation(), AutocheckWarnings::literalSuffixLowerCase);
        break;
      }
    }
  }

  // [A13-6-1] Digit sequences separators ' shall only be used as follows: (1)
  // for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for
  // binary, every 4 digits.
  if (Context.isEnabled(AutocheckWarnings::digitSequenceSeparator) &&
      Tok.is(clang::tok::numeric_constant)) {
    llvm::StringRef Literal(Tok.getLiteralData(), Tok.getLength());

    ParsedNumber ParsedLiteral = parseNumberLiteral(Literal);
    if (!checkDigitSeparators(ParsedLiteral))
      AutocheckDiagnostic::reportWarning(
          DE, Tok.getLocation(), AutocheckWarnings::digitSequenceSeparator);
  }

  // [A6-6-1] The goto statement shall not be used.
  if (Context.isEnabled(AutocheckWarnings::gotoUsed) &&
      Tok.is(clang::tok::kw_goto)) {
    AutocheckDiagnostic::reportWarning(DE, Tok.getLocation(),
                                       AutocheckWarnings::gotoUsed);
  }

  // [A2-11-1] Volatile keyword shall not be used.
  if (Context.isEnabled(AutocheckWarnings::volatileKeywordUsed) &&
      Tok.is(clang::tok::kw_volatile)) {
    AutocheckDiagnostic::reportWarning(DE, Tok.getLocation(),
                                       AutocheckWarnings::volatileKeywordUsed);
  }

  // [A5-2-1] dynamic_cast should not be used.
  if (Context.isEnabled(AutocheckWarnings::dynamicCastUsed) &&
      Tok.is(clang::tok::kw_dynamic_cast)) {
    AutocheckDiagnostic::reportWarning(DE, Tok.getLocation(),
                                       AutocheckWarnings::dynamicCastUsed);
  }

  // [A5-2-4] reinterpret_cast shall not be used.
  if (Context.isEnabled(AutocheckWarnings::reinterpretCastUsed) &&
      Tok.is(clang::tok::kw_reinterpret_cast)) {
    AutocheckDiagnostic::reportWarning(DE, Tok.getLocation(),
                                       AutocheckWarnings::reinterpretCastUsed);
  }
}

// Runs a raw lexer pass on the file and returns a list of tokens.
//
// Implementation taken from clang::syntax::tokenize modified to keep whitespace
// and comments, and return clang::Token instead of clang::syntax::Token.
static std::vector<clang::Token> tokenize(clang::FileID FID,
                                          const clang::SourceManager &SM,
                                          const clang::LangOptions &LO,
                                          clang::IdentifierTable &Identifiers) {
  std::vector<clang::Token> Tokens;
  auto AddToken = [&](clang::Token T) {
    // Fill the proper token kind for keywords, etc.
    if (T.getKind() == clang::tok::raw_identifier && !T.needsCleaning() &&
        !T.hasUCN()) { // TODO: Support needsCleaning and hasUCN cases.
      clang::IdentifierInfo &II = Identifiers.get(T.getRawIdentifier());
      T.setIdentifierInfo(&II);
      T.setKind(II.getTokenID());
    }
    Tokens.push_back(T);
  };

  auto SrcBuffer = SM.getBufferData(FID);
  clang::Lexer L(SM.getLocForStartOfFile(FID), LO, SrcBuffer.data(),
                 SrcBuffer.data(), SrcBuffer.data() + SrcBuffer.size());
  L.SetCommentRetentionState(true);
  L.SetKeepWhitespaceMode(true);

  clang::Token T;
  while (!L.LexFromRawLexer(T) &&
         L.getCurrentBufferOffset() < SM.getFileIDSize(FID))
    AddToken(T);
  // LexFromRawLexer returns true when it parses the last token of the file, add
  // it iff it starts within the range we are interested in.
  if (SM.getFileOffset(T.getLocation()) < SM.getFileIDSize(FID))
    AddToken(T);
  return Tokens;
}

static bool checkDigraphsUsed(const AutocheckContext &Context,
                              const clang::Token &Tok) {
  if (!Context.isEnabled(AutocheckWarnings::digraphsUsed))
    return false;
  return (Tok.isOneOf(clang::tok::l_square, clang::tok::r_square,
                      clang::tok::l_brace, clang::tok::r_brace,
                      clang::tok::hash) &&
          Tok.getLength() == 2) ||
         (Tok.is(clang::tok::hashhash) && Tok.getLength() == 4);
}

static clang::IdentifierInfo *
getDirectiveIdentifierInfo(const llvm::ArrayRef<clang::Token> &Tokens) {
  if (Tokens.size() > 1 && Tokens[0].is(clang::tok::hash) &&
      Tokens[0].isAtStartOfLine()) {
    if (Tokens[1].isAnyIdentifier()) {
      clang::IdentifierInfo *II = Tokens[1].getIdentifierInfo();
      return II;
    }
  }
  return nullptr;
}

bool checkIncludeDirectiveRestrictedChar(
    const AutocheckContext &Context, const llvm::ArrayRef<clang::Token> &Tokens,
    const clang::SourceManager &SM) {
  if (!Context.isEnabled(AutocheckWarnings::includeDirectiveRestrictedChar))
    return false;
  if (Tokens.empty())
    return false;
  // #include has been parsed, now skip all whitespace after it.
  const char *CurrentChar = SM.getCharacterData(Tokens[0].getLocation());
  const char *BufferEnd = &*SM.getBufferData(SM.getMainFileID()).end();
  while (isspace(*CurrentChar) && CurrentChar != BufferEnd)
    CurrentChar++;
  if (CurrentChar == BufferEnd)
    return false;
  // The current character is either '"' or '<'. Now check all characters until
  // the next '"' or '>' respectively.
  assert((*CurrentChar == '"' || *CurrentChar == '<') &&
         "Invalid include directive");
  char ClosingChar = *CurrentChar;
  if (ClosingChar == '<')
    ClosingChar = '>';
  CurrentChar++;
  bool LastCharWasSlash = false;
  while (*CurrentChar != ClosingChar && CurrentChar != BufferEnd) {
    char C = *CurrentChar;
    if (C == '\'' || C == '"' || C == '\\' ||
        (LastCharWasSlash && (C == '/' || C == '*'))) {
      return true;
    }
    LastCharWasSlash = (C == '/');
    CurrentChar++;
  }
  return false;
}

bool checkNonUniversalNames(const AutocheckContext &Context,
                            const llvm::ArrayRef<clang::Token> &Tokens,
                            const clang::SourceManager &SM,
                            unsigned &TokensToSkip) {
  TokensToSkip = 0;
  if (!Context.isEnabled(AutocheckWarnings::nonUniversalNames) ||
      Tokens.empty())
    return false;
  const clang::Token Tok = Tokens[0];
  if (Tok.isAnyIdentifier() && Tok.hasUCN()) {
    return true;
  } else if (Tok.is(clang::tok::unknown) && Tok.hasUCN()) {
    const char *TokenChar = SM.getCharacterData(Tok.getLocation());
    if (*TokenChar == '\\' && tolower(*(TokenChar + 1)) == 'u') {
      // If there are multiple UCNs in a row, display the warning only for
      // the first one and skip all others.
      while (TokensToSkip < Tokens.size() - 1 &&
             Tokens[TokensToSkip + 1].is(clang::tok::unknown) &&
             Tokens[TokensToSkip + 1].hasUCN()) {
        const char *TokenChar =
            SM.getCharacterData(Tokens[TokensToSkip + 1].getLocation());
        if (*TokenChar != '\\' || tolower(*(TokenChar + 1)) != 'u')
          break;
        TokensToSkip++;
      }

      return true;
    }
  }
  return false;
}

void RunRawLexer(const clang::CompilerInstance &CI, clang::FileID FID) {
  const AutocheckContext &Context = AutocheckContext::Get();
  clang::DiagnosticsEngine &DE = CI.getPreprocessor().getDiagnostics();
  clang::SourceManager &SM = CI.getSourceManager();

  // Run a raw lexer pass to check rules which can't be checked with a
  // preprocessor.
  clang::IdentifierTable Identifiers(CI.getLangOpts());
  std::vector<clang::Token> RawTokens =
      tokenize(FID, SM, CI.getLangOpts(), Identifiers);

  clang::Token Tok;
  for (unsigned i = 0; i < RawTokens.size(); i++) {
    Tok = RawTokens[i];
    llvm::ArrayRef<clang::Token> RemainingTokens(RawTokens.data() + i,
                                                 &*RawTokens.end());

    // [A2-3-1] Only those characters specified in the C++ Language Standard
    // basic source character set shall be used in the source code.
    // Special cases:
    // - Comments can contain '@' character
    // - Wide and UTF string/char literals can contain any character
    if (Context.isEnabled(AutocheckWarnings::nonCppStandardCharUsed)) {
      llvm::StringRef TokenString(SM.getCharacterData(Tok.getLocation()),
                                  Tok.getLength());
      // Only tokens that can contain non standard characters which need to be
      // checked are identifiers, regular string literals, regular character
      // literals, comments and unknown tokens. Unknown tokens can be:
      // - whitespace
      // - unknown characters
      // - unterminated comments/strings/chars
      // Unterminated tokens are treated as if they were terminated objects of
      // that type.
      if (clang::tok::isAnyIdentifier(Tok.getKind()) ||
          Tok.isOneOf(clang::tok::string_literal, clang::tok::char_constant,
                      clang::tok::comment, clang::tok::unknown)) {
        bool isComment =
            Tok.is(clang::tok::comment) ||
            (Tok.is(clang::tok::unknown) && TokenString.starts_with("/*"));
        bool WasASCIIChar = true;
        for (int offset = 0; offset < Tok.getLength(); offset++) {
          char C = TokenString[offset];
          if (!(isComment && C == '@') && !isCPlusPlusStandardChar(C)) {
            if (WasASCIIChar || clang::isASCII(C))
              AutocheckDiagnostic::reportWarning(
                  DE, Tok.getLocation().getLocWithOffset(offset),
                  AutocheckWarnings::nonCppStandardCharUsed);
          }
          WasASCIIChar = isASCII(C);
        }
      }
    }

    // [A2-7-1] The character \ shall not occur as a last character of a C++
    // comment.
    if (Context.isEnabled(AutocheckWarnings::lineCommentLastChar) &&
        Tok.is(clang::tok::comment)) {
      const char *CurrentChar = SM.getCharacterData(Tok.getLocation());
      // Check is this a line comment.
      if (CurrentChar[0] == '/' && CurrentChar[1] == '/') {
        const char *CommentEnd = CurrentChar + Tok.getLength();
        unsigned Offset = 0;
        while (CurrentChar != CommentEnd) {
          if (*CurrentChar == '\n' || *CurrentChar == '\r') {
            // We found a newline character, go back to the previous character
            // while skipping any whitespace.
            const char *EscapeChar = CurrentChar - 1;
            unsigned escapeOffset = 1;
            while (clang::isHorizontalWhitespace(*EscapeChar)) {
              EscapeChar--;
              escapeOffset++;
            }
            if (*EscapeChar == '\\')
              AutocheckDiagnostic::reportWarning(
                  DE, Tok.getLocation().getLocWithOffset(Offset - escapeOffset),
                  AutocheckWarnings::lineCommentLastChar);
          }
          CurrentChar++;
          Offset++;
        }
      }
    }

    // [M2-7-1] The character sequence /* shall not be used within a C-style
    // comment.
    if (Context.isEnabled(AutocheckWarnings::commentStartInComment) &&
        Tok.is(clang::tok::comment)) {
      llvm::StringRef CommentString(SM.getCharacterData(Tok.getLocation()),
                                    Tok.getLength());
      if (CommentString.startswith("/*")) {
        size_t idx = 0;
        while ((idx = CommentString.find("/*", idx + 2)) !=
               llvm::StringRef::npos) {
          if (idx + 2 < CommentString.size() && CommentString[idx + 2] != '/')
            AutocheckDiagnostic::reportWarning(
                DE, Tok.getLocation().getLocWithOffset(idx),
                AutocheckWarnings::commentStartInComment);
        }
      }
    }

    // [A2-5-2] Digraphs shall not be used.
    // List of digraphs:
    // Digraph   Equivalent
    // <:        [
    // :>        ]
    // <%        {
    // %>        }
    // %:        #
    // %:%:      ##
    // This is implemented in the raw lexer pass in case digraphs are used
    // inside a macro to display the warning once inside the macro instead of
    // for every expansion of that macro.
    if (checkDigraphsUsed(Context, Tok))
      AutocheckDiagnostic::reportWarning(DE, Tok.getLocation(),
                                         AutocheckWarnings::digraphsUsed);

    // [M16-0-8] If the # token appears as the first token on a line, then it
    // shall be immediately followed by a pre-processing token.
    // [A16-6-1] #error directive shall not be used.
    // This is implemented in the raw lexer pass instead of an Error
    // preprocessor callback because the callback will not be invoked if the
    // directive is skipped (e.g. when its inside a #if 0).
    // [A16-2-1] The ', ", /*, //, \ characters shall not occur in a header file
    // name or in #include directive.
    // This could be implemented in AutocheckPPCallbacks::InclusionDirective
    // except that include files that can't be found don't trigger that callback
    // which makes it hard to test whether this works.
    const clang::IdentifierInfo *II =
        getDirectiveIdentifierInfo(RemainingTokens);
    if (II) {
      switch (II->getPPKeywordID()) {
      case clang::tok::pp_not_keyword:
        if (Context.isEnabled(AutocheckWarnings::preprocessorTokenError))
          AutocheckDiagnostic::reportWarning(
              DE, Tok.getLocation(), AutocheckWarnings::preprocessorTokenError);
        break;
      case clang::tok::pp_error:
        if (Context.isEnabled(AutocheckWarnings::directiveErrorUsed))
          AutocheckDiagnostic::reportWarning(
              DE, Tok.getLocation(), AutocheckWarnings::directiveErrorUsed);
        break;
      case clang::tok::pp_include: {
        if (checkIncludeDirectiveRestrictedChar(Context,
                                                RemainingTokens.slice(2), SM))
          AutocheckDiagnostic::reportWarning(
              DE, Tok.getLocation(),
              AutocheckWarnings::includeDirectiveRestrictedChar);
        break;
      }
      default:
        break;
      }
    }

    // [A2-13-6] Universal character names shall be used only inside character
    // or string literals.
    // Some compilers allow identifiers to start with a UCN, but clang doesn't.
    // Instead, each UCN in the prefix is treated as a separate
    // clang::tok::unknown token. We issue warning for them as well.
    // This is implemented in the raw lexer pass to be able to detect whether
    // multiple UCNs are consecutive or separated by whitespace or comments.
    unsigned TokensToSkip = 0;
    if (checkNonUniversalNames(Context, RemainingTokens, SM, TokensToSkip)) {
      AutocheckDiagnostic::reportWarning(DE, Tok.getLocation(),
                                         AutocheckWarnings::nonUniversalNames);
      i += TokensToSkip;
    }
  }
}

} // namespace lex

} // namespace autocheck
