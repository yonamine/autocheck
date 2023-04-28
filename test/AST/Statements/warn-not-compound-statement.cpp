// RUN: autocheck -verify -Wnot-compound-statement %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {
void test1() {
  std::int32_t a;
  std::int32_t b;

  switch (a) // expected-warning {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
  default:
    b = 0;

  switch (a) {
  case 0:
    b = 0;
    break;
  case 1:
    b = 1;
    break;
  default:
    b = 3;
  }
}

void test2() {
  std::int32_t a;
  std::int32_t b;

  for (std::int32_t i{0}; i < a; ++i) // expected-warning {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
    ++b;

  for (std::int32_t j{0}; j < b; ++j) // expected-warning {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
    ;

  for (std::int32_t i{0}; i < a; ++i) {
    ++b;
  }
}

void test3() {
  std::int32_t a;
  std::int32_t b;

  while (a < 0) // expected-warning {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
    ++b;

  while (a < 0) {
    ++b;
  }

  while (true) // expected-warning {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
    ;
}

void test4() {
  std::int32_t a;
  std::int32_t b;

  do // expected-warning {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
    ++b;
  while (a < 0);

  do {
    ++b;
  } while (a < 0);
}

void test5() {
  std::int32_t arr[5];
  std::int32_t b;

  for (std::int32_t const element : arr) // expected-warning {{The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement}}
    ++b;

  for (std::int32_t const element : arr) {
    ++b;
  }
}
} // namespace

// autosar-warning@10 1 {{variable 'a' is uninitialized when used here}}
// autosar-note@7 1 {{initialize the variable 'a' to silence this warning}}
// autosar-warning@31 1 {{variable 'b' is uninitialized when used here}}
// autosar-note@28 1 {{initialize the variable 'b' to silence this warning}}
// autosar-warning@30 1 {{variable 'a' is uninitialized when used here}}
// autosar-note@27 1 {{initialize the variable 'a' to silence this warning}}
// autosar-warning@46 1 {{variable 'b' is uninitialized when used here}}
// autosar-note@43 1 {{initialize the variable 'b' to silence this warning}}
// autosar-warning@45 1 {{variable 'a' is uninitialized when used here}}
// autosar-note@42 1 {{initialize the variable 'a' to silence this warning}}
// autosar-warning@62 1 {{variable 'a' is uninitialized when used here}}
// autosar-note@57 1 {{initialize the variable 'a' to silence this warning}}
// autosar-warning@61 1 {{variable 'b' is uninitialized when used here}}
// autosar-note@58 1 {{initialize the variable 'b' to silence this warning}}
// autosar-warning@74 1 {{variable 'b' is uninitialized when used here}}
// autosar-note@71 1 {{initialize the variable 'b' to silence this warning}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@60 1 {{Do statements should not be used}}
// autosar-warning@64 1 {{Do statements should not be used}}
// autosar-warning@6 1 {{Unused function 'test1'}}
// autosar-warning@26 1 {{Unused function 'test2'}}
// autosar-warning@41 1 {{Unused function 'test3'}}
// autosar-warning@56 1 {{Unused function 'test4'}}
// autosar-warning@69 1 {{Unused function 'test5'}}
// autosar-warning@70 1 {{C-style arrays shall not be used}}
// autosar-warning@73 1 {{Unused variable 'element'}}
// autosar-warning@76 1 {{Unused variable 'element'}}
// autosar-warning@30 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@33 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@36 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@27 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@42 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@57 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@70 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
