// RUN: autocheck -verify -Wnon-loop-counter-bool %s
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

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@24 1 {{Type 'int32_t' (aka 'int') is "cheap to copy" and should be passed by value}}
// autosar-warning@39 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@43 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@54 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@58 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@62 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@66 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@70 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@44 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@30 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@34 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@39 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
// autosar-warning@39 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@39 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@43 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
// autosar-warning@43 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@50 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@54 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
// autosar-warning@54 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@54 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@58 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
// autosar-warning@58 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@58 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@62 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
// autosar-warning@62 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@62 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@66 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
// autosar-warning@66 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@66 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@70 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
// autosar-warning@70 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@70 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@74 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@29 1 {{Unused function 'test1'}}
// autosar-warning@33 1 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@39 1 {{The comma operator shall not be used}}
// autosar-warning@43 1 {{The comma operator shall not be used}}
// autosar-warning@54 1 {{The comma operator shall not be used}}
// autosar-warning@58 1 {{The comma operator shall not be used}}
// autosar-warning@62 1 {{The comma operator shall not be used}}
// autosar-warning@66 1 {{The comma operator shall not be used}}
// autosar-warning@70 1 {{The comma operator shall not be used}}
