// RUN: autocheck -verify -Wloop-counter-ill-modified %s

#include <cstdint>
#include <vector>

namespace {
void foo1(std::int32_t x) {}
void foo2(const std::int32_t *x) {}
void foo3(std::int32_t &x) {}
void foo4(const std::int32_t &x) {}
void foo5(std::int32_t *x) {}
std::int32_t g;
std::int32_t  fn() { return g;}

void test1() {
  std::int32_t a{0};
  std::int32_t b{0};
  //Misra examples, Rule 6-5-4
  for (std::int32_t i{0}; i < 10; ++i) {
  }
  for (std::int32_t i{0}; i < 10; i++) {
  }
  for (std::int32_t i{0}; i < 10; i += 1) {
  }
  for (std::int32_t i{0}; i < 10; i -= 1) {
  }
  for (std::int32_t i{0}; i < 10; i += a) {
  }
  for (std::int32_t i{0}; i < 10; i -= a) {
  }

  for (std::int32_t i{0}; i < 10; i += a, ++b) {
  }

  for (std::int32_t i{0}; i < 10; i -= b) {
    foo1(b);
  }

  for (std::int32_t i{0}; i < 10; i -= b) {
    foo2(&b);
  }

  for (std::int32_t i{0}; i < 10; i -= b) {
    foo4(b);
  }

  for (std::int32_t i{0}; i < 10; i += a, ++a) { // expected-warning 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
  }

  for (std::int32_t i{0}; i < 10; i *= a) { // expected-warning 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
  }

  for (std::int32_t i{0}; i < 10; i /= a) { // expected-warning 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
  }

  for (std::int32_t i{0}; i < 10; i += b) { // expected-warning 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
    ++b;
  }

  for (std::int32_t i{0}; i < 10; i -= b) { // expected-warning 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
    if (a < 0) {
      ++b;
    }
  }

  for (std::int32_t i{0}; i < 10; i -= b) { // expected-warning 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
    foo3(b);
  }

  for (std::int32_t i{0}; i < 10; i -= b) { // expected-warning 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
    foo5(&b);
  }

  for (std::int32_t i{0}; i < 10; i += fn()) { // expected-warning 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
  }

}

template <typename T>
void f() {
  std::vector<T> thing;
  for ( T x = thing.start(); x != thing.end(); ++x ) {
  }
}

} // namespace
