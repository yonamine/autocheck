// RUN: autocheck -verify -Wcharacter-handling-functions %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@4 0-1 {{There shall be no unused include directives:}} // libstdc++ shows this warning, libc++ does not
// autosar-note@4 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this warning, libc++ does not
// autosar-warning@6 0-1 {{There shall be no unused include directives:}} // libstdc++ shows this warning, libc++ does not
// autosar-note@6 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this warning, libc++ does not
// autosar-warning@7 0-1 {{There shall be no unused include directives:}}  // libc++ shows this warning, libstdc++ does not
// autosar-warning@7 1 {{The C library facilities shall only be accessed through C++ library headers}}
// autosar-warning@11 1 {{Unused function 'RemoveDigits_Bad'}}
// autosar-warning@12 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@13 1 {{Return type of a non-void return type lambda expression should be explicitly specified}}
// autosar-warning@13 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@18 1 {{Unused function 'RemoveDigits_Good'}}
// autosar-warning@19 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@20 1 {{Return type of a non-void return type lambda expression should be explicitly specified}}
// autosar-warning@20 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@21 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@21 1 {{Casts 'unsigned char' to 'int'}}
// autosar-warning@21 1 {{Signed char and unsigned char type shall only be used for the storage and use of numeric values}}
// autosar-warning@21 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@28 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@28 1 {{Unused function 'Count_Preceding_Whitespace_Bad'}}
// autosar-warning@29 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@30  {{The unbounded functions of library <cstring> shall not be used}}
// autosar-warning@30 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@30 1 {{Casts 'int' to 'size_t'}}
// autosar-warning@30 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@31 1 {{Each operand of the ! operator, the logical && or the logical || operators shall have type bool}}
// autosar-warning@31 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@31 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@31 1 {{Casts 'long' to 'size_t'}}
// autosar-warning@34 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@34 1 {{Casts 'long' to 'size_t'}}
// autosar-warning@39 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@39 1 {{Unused function 'Count_Preceding_Whitespace_Compliant'}}
// autosar-warning@40 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@41 1 {{The unbounded functions of library <cstring> shall not be used}}
// autosar-warning@41 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@41 1 {{Casts 'int' to 'size_t'}}
// autosar-warning@41 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@42 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@42 1 {{Each operand of the ! operator, the logical && or the logical || operators shall have type bool}}
// autosar-warning@42 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@42 1 {{Casts 'unsigned char' to 'int'}}
// autosar-warning@42 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@42 1 {{Signed char and unsigned char type shall only be used for the storage and use of numeric values}}
// autosar-warning@42 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@45 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@45 1 {{Casts 'long' to 'size_t'}}
