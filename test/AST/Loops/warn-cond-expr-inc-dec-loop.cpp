// RUN: autocheck -verify -Wcond-expr-inc-dec-loop %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>

namespace {
void test1() {
  //Misra examples, Rule 6-5-2
  for (std::int32_t i{1}; i != 10; i += 2 ) { // expected-warning 1 {{If loop-counter is not modified by -- or ++, then, within condition, the loop-counter shall only be used as an operand to <=, <, > or >=}}
  }
  for (std::int32_t i{1}; i <= 10; i += 2 ) { // Compliant
  }
  for (std::int32_t i{1}; i != 10; ++i ) { // Compliant
  }

  //Additional examples
  for (std::int32_t i{0}; i != 10; i += 1) {// expected-warning 1 {{If loop-counter is not modified by -- or ++, then, within condition, the loop-counter shall only be used as an operand to <=, <, > or >=}}
  }

  for (std::int32_t i{10}; i >= 0; i -= 1) {
  }

  for (std::int32_t i{10}; i != 0; --i) {
  }

  for (std::int32_t i{0}; i < 10;) {
    ++i;
  }

  for (std::int32_t i{0}; i != 10; i += 1) { // expected-warning 1 {{If loop-counter is not modified by -- or ++, then, within condition, the loop-counter shall only be used as an operand to <=, <, > or >=}}
    i = 0;
  }

  for (std::int32_t i{0}; i == 0; i += 1) { // expected-warning 1 {{If loop-counter is not modified by -- or ++, then, within condition, the loop-counter shall only be used as an operand to <=, <, > or >=}}
    i = 0;
  }

  for (std::int32_t i{0}; i == 0; i ++) {
    i = 0;
  }
}
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@7 1 {{Unused function 'test1'}}
// autosar-warning@17 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@26 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@26 1 {{A loop-control-variable other than the loop-counter which is modified in statement shall have type bool}}
// autosar-warning@26 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@30 1 {{The loop-counter shall not be modified within condition or statement}}
// autosar-warning@34 1 {{The loop-counter shall not be modified within condition or statement}}
// autosar-warning@38 1 {{The loop-counter shall not be modified within condition or statement}}
