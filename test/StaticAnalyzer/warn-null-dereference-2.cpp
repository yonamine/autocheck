// RUN: autocheck -verify -Wnull-dereference %s
// RUN: autocheck -verify=expected,autosar -Wall -Wnull-dereference %s

namespace {
void test() {
  int *p = new int(10);
  delete p;
  *p = 10; //expected-warning {{Use of memory after it is freed}}
}
}

// autosar-warning@5 1 {{Unused function 'test'}}
// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@6 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@6 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
