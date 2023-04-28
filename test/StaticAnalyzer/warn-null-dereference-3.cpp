// RUN: autocheck -verify -Wnull-dereference %s
// RUN: autocheck -verify=expected,autosar -Wall -Wnull-dereference %s

namespace {
void test() {
  int *p{nullptr};
  *p = 10; // expected-warning {{Dereference of null pointer}}
}
}

// autosar-warning@5 1 {{Unused function 'test'}}
// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@6 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
