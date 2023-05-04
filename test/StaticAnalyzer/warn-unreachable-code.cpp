// RUN: autocheck -verify -Wunreachable-code %s
// RUN: autocheck -verify=expected,autosar -Wall -Wunreachable-code %s

#include <cstdint>
#include <iostream>

namespace {
int16_t test1(int16_t para) {
  int16_t local;
  local = 0;
  switch (para) {
    local = para; // expected-warning {{A project shall not contain unreachable code}}
  case 1: {
    break;
  }
  default: {
    break;
  }
  }
  return para;
  para++; // expected-warning {{A project shall not contain unreachable code}}
}

void test2() {
  int32_t N{2 + 1};
  if (N == 4) {
    N = 3; // expected-warning {{A project shall not contain unreachable code}}
  }
}

// This test fails for clang UnreachableCode checker
void test3() {
  int32_t x{3}, ind{1};
  while (ind) {
    x++;
  }
  ind++; // expected-warning {{A project shall not contain unreachable code}}
}

void test4(int32_t x) {
  if (x > 5) {
    int32_t z{1};
    int32_t y{x - 2};
    if (y < 3)
      int32_t a{10}; // expected-warning {{A project shall not contain unreachable code}}
  }
}
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 0-1 {{But one or more of it's own #include directives is used}}
// autosar-warning@5 1 {{There shall be no unused include directives:}}
// autosar-warning@8 1 {{Unused function 'test1'}}
// autosar-warning@10 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@24 1 {{Unused function 'test2'}}
// autosar-warning@32 1 {{Unused function 'test3'}}
// autosar-warning@33 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@34 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@40 1 {{Unused function 'test4'}}
// autosar-warning@42 1 {{Unused variable 'z'}}
// autosar-warning@42 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@43 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@44 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@45 1 {{Unused variable 'a'}}
// autosar-warning@45 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
