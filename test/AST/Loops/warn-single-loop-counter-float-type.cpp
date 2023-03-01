// RUN: autocheck -verify -Wsingle-loop-counter-float-type %s

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
  for (std::int32_t i{0}; i > 10; ++i) {
  }
  for (std::int32_t i{0}; i <= 10; ++i) {
  }
  for (std::int32_t i{0}; i >= 10; ++i) {
  }
  for (std::int32_t i{0}; i == 10; ++i) {
  }
  for (std::int32_t i{0}; i != 10; ++i) {
  }
  for (std::int32_t i{0}; i < 10; i += 1) {
  }
  for (std::int32_t i{0}; i > 10; i += 1) {
  }
  for (std::int32_t i{0}; i <= 10; i += 1) {
  }
  for (std::int32_t i{0}; i >= 10; i += 1) {
  }

  std::int32_t n{10};
  for (std::int32_t i{0}; i < n; ++i) {
  }

  std::int32_t j;
  for (; j < 10; ++j) {
  }

  j = 0;
  for (; j < 10; ++j) {
  }

  for (j = 0; j < 10;) { // expected-warning 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
    ++j;
  }

  for (std::int32_t k{0};; ++j) { // expected-warning 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
  }

  for (std::int32_t k{0}; k < 10;) { // expected-warning 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
  }

  std::int32_t l{0};
  for (std::int32_t k{0}; k < 0 && l < 0; ++k, ++l) { // expected-warning 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
  }

  for (float k{0.0F}; k != 1.0F; k += 0.1F) { // expected-warning 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
  }

  for (float k{0.0F}; k != 1.0F;) { // expected-warning 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
  }

  for (float k{0.0F};; k += 0.1F) { // expected-warning 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
  }

  for (std::int32_t i{0}; i >= 10; foo1(i)) { // expected-warning 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
  }

  for (std::int32_t i{0}; i >= 10; foo2(&i)) {
  }

  for (std::int32_t i{0}; i >= 10; foo3(&i)) { // expected-warning 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
  }

  for (std::int32_t i{0}; i >= 10; foo4(i)) {
  }

  for (std::int32_t i{0}; i >= 10; foo5(i)) { // expected-warning 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
  }
}
} // namespace
