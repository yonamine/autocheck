// RUN: autocheck -verify -Wbit-field-unsigned-integral %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@5 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@11 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@29 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@37 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@38 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@40 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@17 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@23 1 {{Enumeration underlying base type shall be explicitly defined}}
// autosar-warning@23 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@29 1 {{Enumerations shall be declared as scoped enum classes}}
// autosar-warning@35 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@41 1 {{Type wchar_t shall not be used}}
