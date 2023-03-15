// RUN: autocheck -verify -Wenum-type-not-defined %s

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
