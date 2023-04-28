// RUN: autocheck -verify -Wchar-storage %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>

namespace {
typedef char char_t;

void test1() {
  char a = 'a';
  char b = '\n';
  char c = 10; // expected-warning {{The plain char type shall only be used for the storage and use of character values}}
  char d = '\t';

  a = 'b';
  b = '\r';
  c = 10;                    // expected-warning {{The plain char type shall only be used for the storage and use of character values}}
  d = static_cast<char>(14); // expected-warning {{The plain char type shall only be used for the storage and use of character values}}

  char i = '3', j = 3, k; // expected-warning {{The plain char type shall only be used for the storage and use of character values}}

  i = 0x31U; // expected-warning {{The plain char type shall only be used for the storage and use of character values}}
  j = '\r';
  k = 4U; // expected-warning {{The plain char type shall only be used for the storage and use of character values}}

  std::uint8_t m;
  m = 10;
  std::int8_t n;
  n = 10;
  char_t o;
  o = 10; // expected-warning {{The plain char type shall only be used for the storage and use of character values}}
  char_t p;
  p = 'a';

  unsigned char q;
  q = 10;
  signed char r;
  r = 10;
}

char test2(char param) {
  return param;
}

char test3() {
  return 15; // expected-warning {{The plain char type shall only be used for the storage and use of character values}}
}

void test4() {
  test2(10);     // expected-warning {{The plain char type shall only be used for the storage and use of character values}}
  char a = 10;   // expected-warning {{The plain char type shall only be used for the storage and use of character values}}
  test2(a = 11); // expected-warning {{The plain char type shall only be used for the storage and use of character values}}
}
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@7 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@10 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@11 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@12 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@13 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@18 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@20 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@35 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@37 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@41 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@45 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@51 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 1 {{The typedef specifier shall not be used}}
// autosar-warning@9 1 {{Unused function 'test1'}}
// autosar-warning@45 1 {{Unused function 'test3'}}
// autosar-warning@49 1 {{Unused function 'test4'}}
// autosar-warning@52 1 {{Assignment operators shall not be used in sub-expressions}}
// autosar-warning@12 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@17 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@20 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@22 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@24 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@27 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@29 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@31 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@36 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@38 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@46 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@50 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@51 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@52 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@10 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@11 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@12 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@13 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@20 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@51 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@22 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@22 1 {{Casts 'unsigned int' to 'char'}}
// autosar-warning@24 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@24 1 {{Casts 'unsigned int' to 'char'}}
// autosar-warning@27 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@27 1 {{Casts 'int' to 'uint8_t'}}
// autosar-warning@36 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@36 1 {{Casts 'int' to 'unsigned char'}}
// autosar-warning@50 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@52 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@20 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
