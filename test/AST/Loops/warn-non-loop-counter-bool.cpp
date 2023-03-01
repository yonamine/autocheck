// RUN: autocheck -verify -Wnon-loop-counter-bool %s

#include <cstdint>

namespace {
void foo1(std::int32_t x) {
  x = 0;
}

void foo2(std::int32_t *x) {
  *x = 0;
}

void foo3(const std::int32_t *x) {
  std::int32_t y{*x};
  ++y;
}

void foo4(std::int32_t &x) {
  x = 0;
}

void foo5(const std::int32_t &x) {
  std::int32_t y{x};
  ++y;
}

void test1() {
  for (std::int32_t i{0}; i < 10; ++i) {
  }

  std::int32_t j{0};
  for (std::int32_t i{0}; i < 10; ++i) {
    j++;
  }

  std::int32_t y{0};
  for (std::int32_t i{0}; i < 0; ++i, ++y) { // expected-warning 1 {{A loop-control-variable other than the loop-counter which is modified in statement shall have type bool}}
    y += 1;
  }

  for (std::int32_t i{0}; i < 0; ++i, ++y) { // expected-warning 1 {{A loop-control-variable other than the loop-counter which is modified in statement shall have type bool}}
    if (i) {
      y++;
    }
  }

  bool z{true};
  for (std::int32_t i{0}; (i < 0) && z; ++i) {
    z = false;
  }

  for (std::int32_t i{0}; i < 0; ++i, ++y) {
    foo1(y);
  }

  for (std::int32_t i{0}; i < 0; ++i, ++y) { // expected-warning 1 {{A loop-control-variable other than the loop-counter which is modified in statement shall have type bool}}
    foo2(&y);
  }

  for (std::int32_t i{0}; i < 0; ++i, ++y) {
    foo3(&y);
  }

  for (std::int32_t i{0}; i < 0; ++i, ++y) { // expected-warning 1 {{A loop-control-variable other than the loop-counter which is modified in statement shall have type bool}}
    foo4(y);
  }

  for (std::int32_t i{0}; i < 0; ++i, ++y) {
    foo5(y);
  }

  for (std::int32_t i{0}; i < 0; ++i) {
    y = 1;
  }
}
} // namespace
