// RUN: autocheck -verify -Wzero-to-null-pointer %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>

namespace {
void int_func(std::int32_t *){};

void test() {
  int_func(0); // expected-warning {{Literal zero (0) shall not be used as the null-pointer-constant}}
  std::int32_t *a;
  a = 0; // expected-warning {{Literal zero (0) shall not be used as the null-pointer-constant}}
}
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives: }}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@10 1 {{Only nullptr literal shall be used as the null-pointer-constant}}
// autosar-warning@12 1 {{Only nullptr literal shall be used as the null-pointer-constant}}
// autosar-warning@9 1 {{Unused function 'test'}}
