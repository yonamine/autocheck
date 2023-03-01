// RUN: autocheck -verify -Wchar-storage %s

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
