// RUN: autocheck -verify -Wnon-loop-counter-modified %s

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
