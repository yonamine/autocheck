// RUN: autocheck -verify -Wzero-to-null-pointer %s

#include <cstdint>

namespace {
void int_func(std::int32_t *){};

void test() {
  int_func(0); // expected-warning {{Literal zero (0) shall not be used as the null-pointer-constant}}
  std::int32_t *a;
  a = 0; // expected-warning {{Literal zero (0) shall not be used as the null-pointer-constant}}
}
} // namespace
