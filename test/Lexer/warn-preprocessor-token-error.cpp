// RUN: autocheck -verify -Wpreprocessor-token-error %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// expected-warning@12 1 {{If the # token appears as the first token on a line, then it shall be immediately followed by a pre-processing token}}
// expected-warning@19 1 {{If the # token appears as the first token on a line, then it shall be immediately followed by a pre-processing token}}

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@7 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@7 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@8 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@8 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
