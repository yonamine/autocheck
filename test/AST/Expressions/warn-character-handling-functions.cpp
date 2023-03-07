// RUN: autocheck -verify -Wcharacter-handling-functions %s

#include <algorithm>
#include <cctype>
#include <string>
#include <string.h>

namespace {
// Test from Autosar document
void RemoveDigits_Bad(std::string &s) {
  s.erase(std::remove_if(s.begin(), s.end(),
                         [](char c) { return std::isdigit(c); }), // expected-warning {{Arguments to character-handling functions shall be representable as an unsigned char}}
          s.cend());
}

// Test from Autosar document
void RemoveDigits_Good(std::string &s) {
  s.erase(std::remove_if(s.begin(), s.end(),
                         [](char c) {
                           return std::isdigit(static_cast<unsigned char>(c));
                         }),
          s.cend());
}

// Test from SEI CERT C++ Coding Standard [9]: STR37-C: Arguments to
// character-handling functions must be representable as an unsigned char.
size_t Count_Preceding_Whitespace_Bad(const char *s) {
  const char *t{s};
  size_t length{strlen(s) + 1};
  while (isspace(*t) && (t - s < length)) { // expected-warning {{Arguments to character-handling functions shall be representable as an unsigned char}}
    ++t;
  }
  return t - s;
}

// Test from SEI CERT C++ Coding Standard [9]: STR37-C: Arguments to
// character-handling functions must be representable as an unsigned char.
size_t Count_Preceding_Whitespace_Compliant(const char *s) {
  const char *t{s};
  size_t length{strlen(s) + 1};
  while (isspace((unsigned char)*t) && (t - s < length)) {
    ++t;
  }
  return t - s;
}
}
