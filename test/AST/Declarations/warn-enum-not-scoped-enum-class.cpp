// RUN: autocheck -verify -Wenum-not-scoped-enum-class %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
enum class Enum1 : int {
  A1,
  B1,
  C1
};

enum class Enum2 {
  A2,
  B2,
  C2
};

enum struct Enum3 : int {
  A1,
  B1,
  C1
};

enum struct Enum4 {
  A2,
  B2,
  C2
};

enum Enum5 : int { // expected-warning {{Enumerations shall be declared as scoped enum classes}}
  A3,
  B3,
  C3
};

enum Enum6 { // expected-warning {{Enumerations shall be declared as scoped enum classes}}
  A4,
  B4,
  C4
};

enum : int { // expected-warning {{Enumerations shall be declared as scoped enum classes}}
  A5,
  B5,
  C5
};

enum { // expected-warning {{Enumerations shall be declared as scoped enum classes}}
  A6,
  B6,
  C6
};

template <typename T> struct A {
  enum Enum7 {}; // expected-warning {{Enumerations shall be declared as scoped enum classes}}
};

template <typename T> struct B {
  enum Enum8 {}; // expected-warning {{Enumerations shall be declared as scoped enum classes}}
};
} // namespace

int main() {
  const A<int> a{};
  const B<int> b{};
  const A<float> c{};
}

// autosar-warning@5 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@17 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@29 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@41 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@11 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@23 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@35 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@47 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@54 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@58 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@63 1 {{Unused variable 'a'}}
// autosar-warning@64 1 {{Unused variable 'b'}}
// autosar-warning@65 1 {{Unused variable 'c'}}
// autosar-warning@63 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@64 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
