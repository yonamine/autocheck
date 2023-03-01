// RUN: autocheck -verify -Wcontainer-loop-ill-formed %s

#include <cstdint>
#include <vector>

namespace {
void test1() {
  std::int32_t x;
  for (std::int32_t i{0}; i < 0; ++i) {
    if (i > 0) {
      x = 0;
    }
  }

  for (std::int32_t i{0}; i < 0; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }

  std::vector<std::int32_t> a;
  for (std::int32_t i{0}; i < a.size(); ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; a.size() < i; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }

  for (std::int32_t i{0}; i < (a.size() / 1); ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; i < (a.size() * 1); ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; i < (a.size() + 0U); ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; i < (a.size() - 0U); ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; (a.size() / 1) < i; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; (a.size() * 1) < i; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; (a.size() + 0U) < i; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; (a.size() - 0U) < i; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }

  for (std::int32_t i{0}; i < (a.size() * (2 - 1)); ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; i < (a.size() / (2 - 1)); ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; i < (a.size() + (1 - 1)); ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; i < (a.size() - (1 - 1)); ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; (a.size() * (2 - 1)) < i; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; (a.size() / (2 - 1)) < i; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; (a.size() + (1 - 1)) < i; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; (a.size() - (1 - 1)) < i; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }

  const std::int32_t q{0};
  const std::int32_t w{1};
  const std::int32_t r{5};
  for (std::int32_t i{0}; i < (a.size() / w); ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; i < (a.size() * w); ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; i < (a.size() + q); ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; i < (a.size() - q); ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; (a.size() / w) < i; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; (a.size() * w) < i; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; (a.size() + q) < i; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; (a.size() - q) < i; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }

  for (std::int32_t i{0}; i < (a.size() * (q + 1)); ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; i < (a.size() / (q + 1)); ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; i < (a.size() + (w - 1)); ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; i < (a.size() - (w - 1)); ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; (a.size() * (q + 1)) < i; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; (a.size() / (q + 1)) < i; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; (a.size() + (w - 1)) < i; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; (a.size() - (w - 1)) < i; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }

  for (std::int32_t i{0}; i < (a.size() * (r + 1)); ++i) {
  }
  for (std::int32_t i{0}; i < (a.size() / (r + 1)); ++i) {
  }
  for (std::int32_t i{0}; i < (a.size() + (r - 1)); ++i) {
  }
  for (std::int32_t i{0}; i < (a.size() - (r - 1)); ++i) {
  }
  for (std::int32_t i{0}; (a.size() * (r + 1)) < i; ++i) {
  }
  for (std::int32_t i{0}; (a.size() / (r + 1)) < i; ++i) {
  }
  for (std::int32_t i{0}; (a.size() + (r - 1)) < i; ++i) {
  }
  for (std::int32_t i{0}; (a.size() - (r - 1)) < i; ++i) {
  }
}

void test2() {
  std::uint32_t a;
  constexpr std::int32_t arraySize{7};
  std::uint32_t array[arraySize]{0, 1, 2, 3, 4, 5, 6};

  for (std::int32_t idx{0}; idx < arraySize; ++idx) // Compliant
  {
    array[idx] = idx;
  }

  for (std::int32_t idx{0}; idx < arraySize; ++idx) // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  {
    a = array[idx];
    array[idx] = 42;
  }

  for (std::int32_t idx{0}; idx < arraySize; ++idx) // Compliant
  {
    array[idx] = array[idx] + array[idx + 1];
  }

  for (std::int32_t idx{0}; idx < arraySize / 2; ++idx) // Compliant - for does not loop through all elements
  {
    a = array[idx];
  }

  for (std::uint32_t *iter{std::begin(array)}; iter != std::cend(array); ++iter) // Compliant
  {
    std::uint32_t *const ptrA{iter};
  }

  for (std::uint32_t *iter{std::begin(array)}; iter != std::cend(array); ++iter) // Compliant
  {
    a = *iter;
    *iter = 42;
    *iter = *iter + 42;
  }

  for (std::uint32_t *iter{std::begin(array)}; iter != std::cend(array); ++iter) // Compliant
  {
    a = *iter + *(iter + 1);
  }

  for (const std::uint32_t value : array) // Compliant - equivalent to non-compliant loops above
  {
    a = value;
  }

  for (std::int32_t idx{0}; idx < arraySize; ++idx) { // Compliant
    if ((idx % 2) == 0) {
      a = array[idx];
    }
  }
}

void test3() {
  for (std::int32_t i{1}; i < 10; ++i) {
  }
  for (std::int32_t i{0}, y{1}; i < y; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{1}, y{0}; i < y; ++i) {
  }

  for (std::int32_t i{0}, y{0}; i < 10; ++i, --y) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{0}; i >= -10; --i) {
  }

  std::int32_t const const_zero{0};
  std::int32_t const const_non_zero{1};
  for (std::int32_t i{const_zero}; i < 10; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
  for (std::int32_t i{const_non_zero}; i < 10; ++i) {
  }
  std::int32_t zero{0};
  std::int32_t non_zero{1};
  for (std::int32_t i{zero}; i < 10; ++i) {
  }
  for (std::int32_t i{non_zero}; i < 10; ++i) {
  }
  for (std::int32_t i{1}, j{2}; i < 10; ++i) {
  }
  for (std::int32_t j{2}, i{0}; i < 10; ++i) { // expected-warning 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
  }
}
} // namespace
