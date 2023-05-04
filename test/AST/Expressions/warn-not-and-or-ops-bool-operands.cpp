// RUN: autocheck -verify -Wnot-and-or-ops-bool-operands %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {
void test() {
  std::int32_t a, b, c, d;
  bool e;

  if (1 && (c < d)) { // expected-warning {{Each operand of the ! operator, the logical && or the logical || operators shall have type bool}}
  }
  if ((a < b) || (c + d)) { // expected-warning {{Each operand of the ! operator, the logical && or the logical || operators shall have type bool}}
  }
  if (!0) { // expected-warning {{Each operand of the ! operator, the logical && or the logical || operators shall have type bool}}
  }
  if (!a) { // expected-warning {{Each operand of the ! operator, the logical && or the logical || operators shall have type bool}}
  }
  if (!(a + b)) { // expected-warning {{Each operand of the ! operator, the logical && or the logical || operators shall have type bool}}
  }

  if ((a < b) && (c < d)) {
  }
  if ((a < b) || (c < d)) {
  }
  if (!false) {
  }
  if (!e) {
  }
  if (!(e || false)) {
  }
}
} // namespace

// autosar-warning@10 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@12 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@14 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@16 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@18 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@27 1 {{variable 'e' is uninitialized when used here}}
// autosar-note@8 1 {{initialize the variable 'e' to silence this warning}}
// autosar-warning@10 1 {{variable 'c' is uninitialized when used here}}
// autosar-note@7 1 {{initialize the variable 'c' to silence this warning}}
// autosar-warning@10 1 {{variable 'd' is uninitialized when used here}}
// autosar-note@7 1 {{initialize the variable 'd' to silence this warning}}
// autosar-warning@12 1 {{variable 'a' is uninitialized when used here}}
// autosar-note@7 1 {{initialize the variable 'a' to silence this warning}}
// autosar-warning@12 1 {{variable 'b' is uninitialized when used here}}
// autosar-note@7 1 {{initialize the variable 'b' to silence this warning}}
// autosar-warning@6 1 {{Unused function 'test'}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@8 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@7 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
