// RUN: autocheck -verify -Wunreachable-code %s

#include <cstdint>
#include <iostream>

namespace {
int16_t test1(int16_t para) {
  int16_t local;
  local = 0;
  switch (para) {
    local = para; // expected-warning {{A project shall not contain unreachable code}}
  case 1: {
    break;
  }
  default: {
    break;
  }
  }
  return para;
  para++; // expected-warning {{A project shall not contain unreachable code}}
}

void test2() {
  int32_t N{2 + 1};
  if (N == 4) {
    N = 3; // expected-warning {{A project shall not contain unreachable code}}
  }
}

// This test fails for clang UnreachableCode checker
void test3() {
  int32_t x{3}, ind{1};
  while (ind) {
    x++;
  }
  ind++; // expected-warning {{A project shall not contain unreachable code}}
}

void test4(int32_t x) {
  if (x > 5) {
    int32_t z{1};
    int32_t y{x - 2};
    if (y < 3)
      int32_t a{10}; // expected-warning {{A project shall not contain unreachable code}}
  }
}
} // namespace
