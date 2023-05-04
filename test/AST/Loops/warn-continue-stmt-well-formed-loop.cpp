// RUN: autocheck -verify -Wcontinue-stmt-well-formed-loop %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>

namespace {
void test() {
  for (std::int32_t i{0}; i < 10; ++i) {
    if (i < 0) {
      continue;
    }
  }

  for (std::int32_t i{0}; i < 10;) {
    for (std::int32_t j{0}; j < 10; ++j) {
      continue;
    }
  }

  for (std::int32_t i{0}; i < 10;) { // expected-warning 1 {{The continue statement shall only be used within a well-formed for loop}}
    for (std::int32_t j{0}; j < 10; ++j) {
      continue;
    }
    continue;
  }

  for (std::int32_t i{0}; i < 10;) { // expected-warning 1 {{The continue statement shall only be used within a well-formed for loop}}
    for (std::int32_t j{0}; j < 10; ++j) {
      continue;
    }
    if (i < 0)
      continue;
  }

  for (std::int32_t i{0}; i < 10;) {
    while (true) {
      continue;
    }
  }

  for (std::int32_t i{0}; i < 10;) { // expected-warning 1 {{The continue statement shall only be used within a well-formed for loop}}
    while (true) {
      continue;
    }
    continue;
  }

  std::int32_t j{0};
  for (std::int32_t i{0}; (i < 0) && (j < i); ++i) { // expected-warning 1 {{The continue statement shall only be used within a well-formed for loop}}
    j--;
    if (i == -1) {
      continue;
    }
  }
}
} // namespace

// autosar-warning@14 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@20 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@27 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@35 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@41 1 {{A for loop shall contain a single loop-counter which shall not have floating-point type}}
// autosar-warning@49 1 {{A loop-control-variable other than the loop-counter which is modified in statement shall have type bool}}
// autosar-warning@31 1 {{An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@14 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@15 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@20 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@21 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@27 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@28 1 {{A for-loop that does not use its loop-counter (in a way that prevents the substitution of the loop with a range-based for-loop) shall not be used}}
// autosar-warning@35 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@41 1 {{For-init-statement and expression should not perform actions other than loop-counter initialization and modification}}
// autosar-warning@7 1 {{Unused function 'test'}}
// autosar-warning@14 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@20 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@27 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@35 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@41 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@48 1 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
