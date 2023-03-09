// RUN: autocheck -verify -Wcond-expr-inc-dec-loop %s

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
