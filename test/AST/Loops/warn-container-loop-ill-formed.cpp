// RUN: autocheck -verify -Wcontainer-loop-ill-formed %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@5 0-1 {{There shall be no unused include directives:}} // libstc++ shows this warning, libc++ does not
// autosar-note@5 0-1 {{But one or more of it's own #include directives is used}} // libstc++ shows this note, libc++ does not
// autosar-warning@8 1 {{Unused function 'test1'}}
// autosar-warning@114 1 {{Unused function 'test2'}}
// autosar-warning@169 1 {{Unused function 'test3'}}
// autosar-warning@20 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@20 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@22 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@22 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@25 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@25 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@27 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@27 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@29 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@29 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@31 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@31 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@33 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@33 1 {{Casts 'int' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@35 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@35 1 {{Casts 'int' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@37 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@37 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@39 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@39 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@42 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@42 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@44 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@44 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@46 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@46 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@48 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@48 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@50 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@50 1 {{Casts 'int' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@52 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@52 1 {{Casts 'int' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@54 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@54 1 {{Casts 'int' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@56 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@56 1 {{Casts 'int' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@62 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@62 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@64 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@64 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@66 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@66 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@68 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@68 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@70 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@70 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@72 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@72 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@74 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@74 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@76 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@76 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@79 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@79 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@81 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@81 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@83 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@83 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@85 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@85 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@87 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@87 1 {{Casts 'int' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@89 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@89 1 {{Casts 'int' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@91 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@91 1 {{Casts 'int' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@93 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@93 1 {{Casts 'int' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@96 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@96 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@98 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@98 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@100 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@100 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@102 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@102 1 {{Casts 'int32_t' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@104 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@104 1 {{Casts 'int' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@106 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@106 1 {{Casts 'int' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@108 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@108 1 {{Casts 'int' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@110 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@110 1 {{Casts 'int' to 'std::vector<int, std::allocator<int>>::size_type'}}
// autosar-warning@117 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@117 1 {{Casts 'int' to 'uint32_t'}}
// autosar-warning@121 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@121 1 {{Casts 'int32_t' to 'uint32_t'}}
// autosar-warning@127 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@127 1 {{Casts 'int' to 'uint32_t'}}
// autosar-warning@148 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@148 1 {{Casts 'int' to 'uint32_t'}}
// autosar-warning@149 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@149 1 {{Casts 'int' to 'uint32_t'}}
// autosar-warning@19 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@172 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@174 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@59 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@60 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@61 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@116 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@157 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@9 1 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@117 1 {{C-style arrays shall not be used}}
// autosar-warning@142 1 {{Unused variable 'ptrA'}}
// autosar-warning@140 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@145 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@152 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@172 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@174 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@177 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@184 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@186 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@188 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@189 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@190 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@192 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@177 1 {{The comma operator shall not be used}}
// autosar-warning@177 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@177 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
// autosar-warning@194 1 {{Unused variable 'j'}}
// autosar-warning@196 1 {{Unused variable 'j'}}
// autosar-warning@194 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@196 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@194 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@196 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
