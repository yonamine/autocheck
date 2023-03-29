// RUN: autocheck -verify -Wnot-and-or-ops-bool-operands %s

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
