// RUN: autocheck -verify -Wnull-dereference %s
// RUN: autocheck -verify=expected,autosar -Wall -Wnull-dereference %s

void test() {
  int *p;
  *p = 10; // expected-warning {{Dereference of undefined pointer value}}
}

// autosar-warning@6 1 {{variable 'p' is uninitialized when used here}}
// autosar-note@5 1 {{initialize the variable 'p' to silence this warning}}
// autosar-warning@5 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@4 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@5 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
