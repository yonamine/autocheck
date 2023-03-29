// RUN: autocheck -verify -Wenum-not-scoped-enum-class %s

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
