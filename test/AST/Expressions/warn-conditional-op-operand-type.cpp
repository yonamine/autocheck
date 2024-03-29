// RUN: autocheck -verify -Wconditional-op-operand-type %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@5 1 {{Unused function 'test'}}
// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@8 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@6 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
