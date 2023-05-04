// RUN: autocheck -verify -Wsingle-loop-counter-float-type %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

  std::int32_t j{0};
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

// autosar-warning@67 1 {{Unused variable 'k'}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@24 1 {{Type 'int32_t' (aka 'int') is "cheap to copy" and should be passed by value}}
// autosar-warning@74 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@30 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@32 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@34 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@36 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@38 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@40 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@42 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@44 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@46 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@48 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@52 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@56 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@60 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@63 1 {{A loop-control-variable other than the loop-counter which is modified in statement shall have type bool}}
// autosar-warning@63 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@67 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
// autosar-warning@67 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@70 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@74 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@77 1 {{If loop-counter is not modified by -- or ++, then, within condition, the loop-counter shall only be used as an operand to <=, <, > or >=}}
// autosar-warning@80 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@83 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
// autosar-warning@83 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@86 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@89 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@89 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
// autosar-warning@92 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@95 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@95 1 {{The loop-counter shall be modified by one of: --, ++, -= n, or += n; where n remains constant for the duration of the loop}}
// autosar-warning@98 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@29 1 {{Unused function 'test1'}}
// autosar-warning@51 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@67 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@70 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@80 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@86 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@98 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@51 1 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@73 1 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@74 1 {{The comma operator shall not be used}}
