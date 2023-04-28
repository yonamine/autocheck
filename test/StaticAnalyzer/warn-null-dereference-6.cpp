// RUN: autocheck -verify -Wnull-dereference %s
// RUN: autocheck -verify=expected,autosar -Wall -Wnull-dereference %s

#include <cstdint>
namespace {
void test() {
  int32_t x{10};
  int32_t y;
  int32_t *p{nullptr};
  if (y)
    p = &x;
  *p = 10; // expected-warning {{Dereference of null pointer (loaded from variable 'p')}}
}
}

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@6 1 {{Unused function 'test'}}
// autosar-warning@7 1 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@10 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@10 1 {{variable 'y' is uninitialized when used here}}
// autosar-note@8 1 {{initialize the variable 'y' to silence this warning}}
// autosar-warning@10 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@8 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
