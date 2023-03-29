// RUN: autocheck -verify -Wc-style-cast-used %s
// Test taken from clang/test/SemaCXX/old-style-cast.cpp

void test1() {
  long x = (long)12; // expected-warning {{Traditional C-style casts shall not be used}}
  (long)x;           // expected-warning {{Traditional C-style casts shall not be used}}
  (void**)x;         // expected-warning {{Traditional C-style casts shall not be used}}
  long y = static_cast<long>(12);
  (void)y;
  typedef void VOID;
  (VOID)y;
}
