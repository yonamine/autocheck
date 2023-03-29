// RUN: autocheck -verify -Wbit-field-unsigned-integral %s

namespace {
enum class Enum1 : unsigned int {
  E1,
  E2,
  E3
};

enum class Enum2 : int {
  E1,
  E2,
  E3
};

enum class Enum3 {
  E1,
  E2,
  E3
};

enum Enum4 {
  E1,
  E2,
  E3
};

enum Enum5 : unsigned {
  E4,
  E5,
  E6
};

class A {
public:
  int a : 2; // expected-warning 1 {{Bit-fields shall be either unsigned integral, or enumeration (with underlying type of unsigned integral type)}}
  unsigned int b : 2U;
  bool c : 1;    // expected-warning 1 {{Bit-fields shall be either unsigned integral, or enumeration (with underlying type of unsigned integral type)}}
  char d : 2;    // expected-warning 1 {{Bit-fields shall be either unsigned integral, or enumeration (with underlying type of unsigned integral type)}}
  wchar_t e : 2; // expected-warning 1 {{Bit-fields shall be either unsigned integral, or enumeration (with underlying type of unsigned integral type)}}
  Enum1 f : 2;
  Enum2 g : 2; // expected-warning 1 {{Bit-fields shall be either unsigned integral, or enumeration (with underlying type of unsigned integral type)}}
  Enum3 h : 2; // expected-warning 1 {{Bit-fields shall be either unsigned integral, or enumeration (with underlying type of unsigned integral type)}}
  Enum4 i : 2; // expected-warning 1 {{Bit-fields shall be either unsigned integral, or enumeration (with underlying type of unsigned integral type)}}
  Enum5 j : 2;
};
} // namespace
