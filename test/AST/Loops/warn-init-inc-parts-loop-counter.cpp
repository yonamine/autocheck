// RUN: autocheck -verify -Winit-inc-parts-loop-counter %s

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
