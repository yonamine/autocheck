// RUN: autocheck -verify -Wenum-type-not-defined %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
enum Enum1 { // expected-warning {{Enumeration underlying base type shall be explicitly defined}}
  A1,
  B1,
  C1
};

enum Enum2 : int {
  A2,
  B2,
  C2
};

enum class Enum3 : int {
  A3,
  B3,
  C3
};

enum class Enum4 { // expected-warning {{Enumeration underlying base type shall be explicitly defined}}
  A4,
  B4,
  C4
};

enum { // expected-warning {{Enumeration underlying base type shall be explicitly defined}}
  A5,
  B5,
  C5
};

enum : int {
  A6,
  B6,
  C6
};

void use_test() {
  enum Enum1 x;
  enum Enum3 y;
}
} // namespace

// autosar-warning@11 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@17 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@35 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@5 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@11 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@29 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@35 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@41 1 {{Unused function 'use_test'}}
// autosar-warning@42 1 {{Unused variable 'x'}}
// autosar-warning@43 1 {{Unused variable 'y'}}
// autosar-warning@42 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@43 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
