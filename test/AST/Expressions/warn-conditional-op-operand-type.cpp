// RUN: autocheck -verify -Wconditional-op-operand-type %s

namespace {
void test() {
  int a{0};
  bool b{false};
  int c{(b && true) ? 0 : 1};
  c = (a < 0) ? 0 : 1;
  c = a ? 0 : 1;       // expected-warning {{The first operand of a conditional-operator shall have type bool}}
  c = (a + 1) ? 0 : 1; // expected-warning {{The first operand of a conditional-operator shall have type bool}}
}
} // namespace
