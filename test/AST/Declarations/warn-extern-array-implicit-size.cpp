// RUN: autocheck -verify -Wextern-array-implicit-size %s

namespace {
extern int a1[]; // expected-warning {{When an array with external linkage is declared, its size shall be stated explicitly}}
extern int a2[2];
extern int a3[2+2];
extern int a4[] = {1, 2}; // expected-warning {{When an array with external linkage is declared, its size shall be stated explicitly}}
extern int a5[] {1, 2}; // expected-warning {{When an array with external linkage is declared, its size shall be stated explicitly}}

void test() {
  extern int b1[]; // expected-warning {{When an array with external linkage is declared, its size shall be stated explicitly}}
  extern int b2[2];
  extern int b3[2+2];
}
} // namespace
