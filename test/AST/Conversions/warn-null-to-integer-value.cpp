// RUN: autocheck -verify -Wnull-to-integer-value %s

#include <cstddef>
#include <cstdint>

namespace {
void ptr_func(std::int32_t){};

void test1() {
  ptr_func(NULL); // expected-warning {{NULL shall not be used as an integer value}}
  std::int32_t a;
  a = NULL; // expected-warning {{NULL shall not be used as an integer value}}
}
} // namespace
