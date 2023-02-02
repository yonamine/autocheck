//===--- CharHelper.cpp - Helper functions for character constants --------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements helper functions for character constants.
//
//===----------------------------------------------------------------------===//

#include "Lex/CharHelper.h"

#include <cctype>

namespace autocheck {

bool isASCII(char Char) {
  return static_cast<unsigned char>(Char) <= 127;
}

bool isCPlusPlusStandardChar(char Char) {
  switch (Char) {
  case ' ':
  case '\t':
  case '\v':
  case '\f':
  case '\n':
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'G':
  case 'H':
  case 'I':
  case 'J':
  case 'K':
  case 'L':
  case 'M':
  case 'N':
  case 'O':
  case 'P':
  case 'Q':
  case 'R':
  case 'S':
  case 'T':
  case 'U':
  case 'V':
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'g':
  case 'h':
  case 'i':
  case 'j':
  case 'k':
  case 'l':
  case 'm':
  case 'n':
  case 'o':
  case 'p':
  case 'q':
  case 'r':
  case 's':
  case 't':
  case 'u':
  case 'v':
  case 'w':
  case 'x':
  case 'y':
  case 'z':
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  case '_':
  case '{':
  case '}':
  case '[':
  case ']':
  case '#':
  case '(':
  case ')':
  case '<':
  case '>':
  case '%':
  case ':':
  case ';':
  case '.':
  case '?':
  case '*':
  case '+':
  case '-':
  case '/':
  case '^':
  case '&':
  case '|':
  case '~':
  case '!':
  case '=':
  case ',':
  case '\\':
  case '"':
  case '\'':
    return true;
  default:
    return false;
  }
}

bool isValidCommentChar(char Char) {
  return Char == '@' || isCPlusPlusStandardChar(Char);
}

bool isValidEscapeChar(char Char) {
  switch (Char) {
  case '\'':
  case '\"':
  case '?':
  case '\\':
  case 'a':
  case 'b':
  case 'f':
  case 'n':
  case 'r':
  case 't':
  case 'v':
  case 'x':
  case 'u':
  case 'U':
    return true;
  default:
    return isOctalDigit(Char);
  }
}

bool isHexDigit(char Char) {
  return (Char >= '0' && Char <= '9') ||
         (toupper(Char) >= 'A' && toupper(Char) <= 'F');
}

bool isOctalDigit(char Char) { return Char >= '0' && Char <= '7'; }

} // namespace autocheck
