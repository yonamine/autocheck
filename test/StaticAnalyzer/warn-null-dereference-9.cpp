// RUN: autocheck -verify -Wnull-dereference %s
// RUN: autocheck -verify=expected,autosar -Wall -Wnull-dereference %s

#include <cstdint>

int32_t f(int32_t **p, int32_t k);
void test(int32_t *p, int32_t k) {
  k = -10000;
  while (k!=-1) {
    k = f(&p, k);
    if(k==-1) p = nullptr;
  }
  int32_t x{p[0]}; // expected-warning {{Array access (from variable 'p') results in a null pointer dereference}}
}

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@6 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@7 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@11 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@11 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@13 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 1 {{Unused variable 'x'}}
