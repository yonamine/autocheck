// RUN: autocheck -verify -Wcondition-not-bool %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
#include <memory>

namespace {
std::int32_t* foo() {
  return nullptr;
}

bool foo2(bool x) {
  return x;
}

void test1() {
  bool a;
  std::int32_t b;

  if (a) {}
  if (a == true) {}
  if (b < 0) {}
  if (b && (a < true)) {} // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
  if (b) {} // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
  if (std::int32_t *c{foo()}) {}

  std::unique_ptr<std::int32_t> uptr{};
  if (uptr) {}

  while (a) {}
  while (a == true) {}
  while (b < 0) {}
  while (b && (a < true)) {} // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
  while (b) {} // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
  while (std::int32_t *c{foo()}) {}

  for (; a; ) {}
  for (; a == true; ) {}
  for (; b < 0; ) {}
  for (; b && (a < true); ) {} // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
  for (; b; ) {} // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
  for (; std::int32_t *c{foo()}; ) {}

  do {} while (a);
  do {} while (a == true);
  do {} while (b < 0);
  do {} while (b && (a < true)); // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
  do {} while (b); // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}

  if (foo2(static_cast<bool>(1))) {}
  if (foo2(1)) {} // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}

  if (a) {
    if (b) { // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
    }
  }

  while (a) {
    while (b) { // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
    }
  }

  for (; a;) {
    for (; b;) { // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
    }
  }

  do {
    do {
    } while (b); // expected-warning {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
  } while (a);
}
}

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@5 0-1 {{There shall be no unused include directives:}} // libstdc++ shows this warning, libc++ does not
// autosar-note@5 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this note, libc++ does not
// autosar-warning@16 1 {{Unused function 'test1'}}
// autosar-warning@23 1 {{Each operand of the ! operator, the logical && or the logical || operators shall have type bool}}
// autosar-warning@33 1 {{Each operand of the ! operator, the logical && or the logical || operators shall have type bool}}
// autosar-warning@40 1 {{Each operand of the ! operator, the logical && or the logical || operators shall have type bool}}
// autosar-warning@47 1 {{Each operand of the ! operator, the logical && or the logical || operators shall have type bool}}
// autosar-warning@23 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@33 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@40 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@47 1 {{Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator = , the logical operators && , || , ! , the equality operators == and != , the unary & operator, and the conditional operator}}
// autosar-warning@22 1 {{variable 'b' is uninitialized when used here}}
// autosar-note@18 1 {{initialize the variable 'b' to silence this warning}}
// autosar-warning@20 1 {{variable 'a' is uninitialized when used here}}
// autosar-note@17 1 {{initialize the variable 'a' to silence this warning}}
// autosar-warning@44 1 {{Do statements should not be used}}
// autosar-warning@45 1 {{Do statements should not be used}}
// autosar-warning@46 1 {{Do statements should not be used}}
// autosar-warning@47 1 {{Do statements should not be used}}
// autosar-warning@48 1 {{Do statements should not be used}}
// autosar-warning@68 1 {{Do statements should not be used}}
// autosar-warning@69 1 {{Do statements should not be used}}
// autosar-warning@37 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@38 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@39 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@40 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@41 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@42 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@63 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@64 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@17 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@18 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@27 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@25 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@35 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@42 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
