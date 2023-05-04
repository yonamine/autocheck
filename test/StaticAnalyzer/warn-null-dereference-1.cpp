// RUN: autocheck -verify -Wnull-dereference %s
// RUN: autocheck -verify=expected,autosar -Wall -Wnull-dereference %s

#include <cstdint>
namespace {
int32_t f(int32_t **p, int32_t k) { return ++k; }

void test(int32_t *p, int32_t k) {
  if (p)
    return;
  k = -3;
  while (k) {
    k = f(&p, k);
  }
  int32_t x = p[0]; // expected-warning {{Array access (from variable 'p') results in a null pointer dereference}}
}
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@6 1 {{Unused parameter 'p'}}
// autosar-warning@6 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@6 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@8 1 {{Unused function 'test'}}
// autosar-warning@9 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@12 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@15 1 {{Unused variable 'x'}}
// autosar-warning@15 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@15 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
