// RUN: autocheck -verify -Wliteral-suffix-lower-case %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
void test() {
  long a{0L};
  a = 0l; // expected-warning 1 {{Literal suffixes shall be upper case}}

  unsigned long b{0U};
  b = 0xAU;
  b = 0u;    // expected-warning 1 {{Literal suffixes shall be upper case}}
  b = 0x0Au; // expected-warning 1 {{Literal suffixes shall be upper case}}

  unsigned long c{0UL};
  c = 0Ul; // expected-warning 1 {{Literal suffixes shall be upper case}}
  c = 0uL; // expected-warning 1 {{Literal suffixes shall be upper case}}
  c = 0ul; // expected-warning 1 {{Literal suffixes shall be upper case}}

  float d{0.0F};
  d = 0.0f; // expected-warning 1 {{Literal suffixes shall be upper case}}
}
} // namespace

// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@9 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@14 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@5 1 {{Unused function 'test'}}
