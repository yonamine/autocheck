// RUN: autocheck -verify -Wnull-dereference %s
// RUN: autocheck -verify=expected,autosar -Wall -Wnull-dereference %s

#include <cstdint>
namespace {
void function(int32_t *p) {
  *p = 10;  // expected-warning {{Dereference of null pointer}}
}

void test() {
  int32_t *p{nullptr};
  function(p);
}
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@10 1 {{Unused function 'test'}}
// autosar-warning@11 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
