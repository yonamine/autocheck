// RUN: autocheck -verify -Wpreprocessor-token-error %s

#include <cstdint>

#define AAA 2
int foo(void) {
  int32_t x = 0;

#ifndef AAA
  x = 1;
#klse1
  x = AAA;
#endif
  return x;

#ifdef BBB
x = 1;
#elifS 2
x = AAA;
#endif
}

// expected-warning@11 1 {{If the # token appears as the first token on a line, then it shall be immediately followed by a pre-processing token}}
// expected-warning@18 1 {{If the # token appears as the first token on a line, then it shall be immediately followed by a pre-processing token}}
