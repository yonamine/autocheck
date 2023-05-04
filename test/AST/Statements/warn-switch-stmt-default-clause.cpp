// RUN: autocheck -verify -Wswitch-stmt-default-clause %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {
void test() {
  std::int32_t condition;

  switch (condition) {
  case 0:
    break;
  case 1:
    break;
  default:
    break;
  }

  switch (condition) {
  case 0:
    break;
  case 1:
    switch (condition) {
    case 0:
      break;
    case 1:
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }

  switch (condition) {
  case 0:
    break;
  case 1:
  default:
    break;
  }

  switch (condition) {
  default:
    break;
  }

  switch (condition) {} // expected-warning 1 {{The final clause of a switch statement shall be the default-clause}}

  switch (condition) { // expected-warning 1 {{The final clause of a switch statement shall be the default-clause}}
  case 0:
    break;
  case 1:
    break;
  case 2:
    break;
  }

  switch (condition) { // expected-warning 1 {{The final clause of a switch statement shall be the default-clause}}
  case 0:
    break;
  default:
    break;
  case 1:
    break;
  }

  switch (condition) { // expected-warning 1 {{The final clause of a switch statement shall be the default-clause}}
  default:
    break;
  case 0:
    break;
  case 1:
    break;
  }

  switch (condition) {
  case 0:
    break;
  case 1:
    switch (condition) { // expected-warning 1 {{The final clause of a switch statement shall be the default-clause}}
    case 0:
      break;
    default:
      break;
    case 1:
      break;
    }
    break;
  default:
    break;
  }

  switch (condition) { // expected-warning 1 {{The final clause of a switch statement shall be the default-clause}}
  case 0:
    break;
  case 1:
    switch (condition) { // expected-warning 1 {{The final clause of a switch statement shall be the default-clause}}
    default:
      break;
    case 0:
      break;
    case 1:
      break;
    }
    break;
  }

  switch (condition) { // expected-warning 1 {{The final clause of a switch statement shall be the default-clause}}
  default:
  case 1:
  case 2:
    break;
  }

  switch (condition) { // expected-warning 1 {{The final clause of a switch statement shall be the default-clause}}
  case 1:
  case 2:
  default:
  case 3:
  case 4:
    break;
  }

  enum class EClass : std::int32_t {
    ENUM_1,
    ENUM_2,
    ENUM_3
  };
  EClass enums;

  switch (enums) {
  case EClass::ENUM_1:
    break;
  case EClass::ENUM_2:
    break;
  case EClass::ENUM_3:
    break;
  }

  switch (enums) {
  case EClass::ENUM_1:
    break;
  case EClass::ENUM_2:
    switch (enums) {
    case EClass::ENUM_1:
      break;
    case EClass::ENUM_2:
      break;
    case EClass::ENUM_3:
      break;
    }
    break;
  case EClass::ENUM_3:
    break;
  }

  switch (enums) {
  case EClass::ENUM_1:
    break;
  case EClass::ENUM_2:
    break;
  case EClass::ENUM_3:
    break;
  default:
    break;
  }

  switch (enums) {
  case EClass::ENUM_1:
    break;
  case EClass::ENUM_2:
    switch (condition) {
    case 1:
      break;
    case 2:
      break;
    default:
      break;
    }
    break;
  case EClass::ENUM_3:
    break;
  }

  switch (enums) {
  case EClass::ENUM_1:
    break;
  case EClass::ENUM_2:
    break;
  default:
    break;
  }

  switch (enums) { // expected-warning 1 {{The final clause of a switch statement shall be the default-clause}}
  case EClass::ENUM_1:
    break;
  case EClass::ENUM_2:
    break;
  default:
    break;
  case EClass::ENUM_3:
    break;
  }

  switch (enums) { // expected-warning 1 {{The final clause of a switch statement shall be the default-clause}}
  case EClass::ENUM_1:
    break;
  case EClass::ENUM_2:
    break;
  }

  switch (condition) {
  case 1:
    switch (condition) { // expected-warning 1 {{The final clause of a switch statement shall be the default-clause}}
    case 1:
      break;
    default:
      break;
    case 2:
      break;
    }
    break;
  case 2:
    switch (enums) { // expected-warning 1 {{The final clause of a switch statement shall be the default-clause}}
    case EClass::ENUM_1:
      break;
    case EClass::ENUM_2:
      break;
    }
    break;
  default:
    break;
  }
}
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@132 1 {{variable 'enums' is uninitialized when used here}}
// autosar-note@130 1 {{variable 'enums' is declared here}}
// autosar-warning@9 1 {{variable 'condition' is uninitialized when used here}}
// autosar-note@7 1 {{initialize the variable 'condition' to silence this warning}}
// autosar-warning@6 1 {{Unused function 'test'}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@130 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
