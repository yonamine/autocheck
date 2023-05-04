// RUN: autocheck -verify -Wnon-loop-counter-modified %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>

namespace {
std::int32_t foo1(std::int32_t x) {
  return x;
}

std::int32_t foo2(std::int32_t *x) {
  return *x;
}

std::int32_t foo3(const std::int32_t *x) {
  return *x;
}

std::int32_t foo4(std::int32_t &x) {
  return x;
}

std::int32_t foo5(const std::int32_t &x) {
  return x;
}

void test1() {
  std::int32_t y{1};
  for (std::int32_t x{0}; (++y < 0) && (x < 0); ++x) { // expected-warning 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
  }

  for (std::int32_t x{0}; x < 0; ++x, y++) { // expected-warning 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
  }

  for (std::int32_t x{0}; (foo1(y) < 0) && (x < 0); ++x) {
  }

  for (std::int32_t x{0}; (foo1(y++) < 0) && (x < 0); ++x) { // expected-warning 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
  }

  for (std::int32_t x{0}; (foo2(&y) < 0) && (x < 0); ++x) { // expected-warning 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
  }

  for (std::int32_t x{0}; (foo3(&y) < 0) && (x < 0); ++x) {
  }

  for (std::int32_t x{0}; (foo4(y) < 0) && (x < 0); ++x) { // expected-warning 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
  }

  for (std::int32_t x{0}; (foo5(y) < 0) && (x < 0); ++x) {
  }
}
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@23 1 {{Type 'int32_t' (aka 'int') is "cheap to copy" and should be passed by value}}
// autosar-warning@29 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@32 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@29 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@32 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@32 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@35 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@38 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@41 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@44 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@47 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@50 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@27 1 {{Unused function 'test1'}}
// autosar-warning@32 1 {{The comma operator shall not be used}}
