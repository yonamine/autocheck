// RUN: autocheck -verify -Winit-inc-parts-loop-counter %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>

namespace {
void test1() {
  for (std::int32_t i{0}, y{1}; i < y; ++i) { // expected-warning 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
  }

  std::int32_t z{0};
  for (std::int32_t i{0}; i < 0; ++i, ++z) { // expected-warning 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
  }

  for (std::int32_t i{0}; i < 0; ++i) {
  }

  std::int32_t j{0};
  for (++j; j < 0; ++j) { // expected-warning 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
  }

  for (std::int32_t i{0}; i < 0; i) { // expected-warning 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
  }
}

std::int32_t f(std::int32_t y) {
  return y;
}

void test2() {
  std::int32_t a;
  std::int32_t const b{5};

  for(a=b; a<10; a++) {
  }

  for(a=f(b); a<10; a++) { // expected-warning 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
  }

  for(a=b+1; a<10; a++) { // expected-warning 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
  }

  for(a=f(b)+1; a<10; a++) { // expected-warning 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
  }

  for(a+=b; a<10; a++) { // expected-warning 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
  }

  for(a=a+1; a<10; a++) { // expected-warning 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
  }

  for(std::int32_t a1 = b; a1<10; a1++) { // expected-warning 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
  }

  for(std::int32_t a1{b}; a1<10; a1++) { // expected-warning 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
  }

  for(std::int32_t a1=b+1; a1<10; a1++) { // expected-warning 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
  }

  for(a=f(a); a<10; a++) { // expected-warning 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
  }

  for(a=a+1; a<10; a++) { // expected-warning 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
  }

  for(a+1; a<10; a++) { // expected-warning 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
  }

  for(a+=0; a<10; a++) { // expected-warning 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
  }

  for(a=0, a=0; a<10; a++) { // expected-warning 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
  }
}
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@8 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@12 1 {{The increment (++) and decrement (--) operators shall not be mixed with other operators in an expression}}
// autosar-warning@8 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@12 1 {{A loop-control-variable other than the loop-counter shall not be modified within condition or expression}}
// autosar-warning@12 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@15 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@19 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@22 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@7 1 {{Unused function 'test1'}}
// autosar-warning@22 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@11 1 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@12 1 {{The comma operator shall not be used}}
// autosar-warning@30 1 {{Unused function 'test2'}}
// autosar-warning@52 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@58 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@73 1 {{The comma operator shall not be used}}
// autosar-warning@73 1 {{Assignment operators shall not be used in sub-expressions}}
