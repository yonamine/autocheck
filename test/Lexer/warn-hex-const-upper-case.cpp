// RUN: autocheck -verify -Whex-const-upper-case %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
void test() {
  long a1{0xAb2A2}; // expected-warning 1 {{Hexadecimal constants should be upper case}}
  long a2{0xAB2A2};

  long b1 = 0x3c4p2; // expected-warning 1 {{Hexadecimal constants should be upper case}}
  long b2 = 0x3C4p2;

  float c1{0x.a2D2p2}; // expected-warning 1 {{Hexadecimal constants should be upper case}}
  float c2{0x.A2D2p2};

  float d1{0xA3.a3p2}; // expected-warning 1 {{Hexadecimal constants should be upper case}}
  float d2{0xa3.A3p2}; // expected-warning 1 {{Hexadecimal constants should be upper case}}
  float d3{0xA3.A3p2};
}
} // namespace

// autosar-warning@6 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@9 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@10 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@6 1 {{Unused variable 'a1'}}
// autosar-warning@7 1 {{Unused variable 'a2'}}
// autosar-warning@9 1 {{Unused variable 'b1'}}
// autosar-warning@10 1 {{Unused variable 'b2'}}
// autosar-warning@12 1 {{Unused variable 'c1'}}
// autosar-warning@13 1 {{Unused variable 'c2'}}
// autosar-warning@15 1 {{Unused variable 'd1'}}
// autosar-warning@16 1 {{Unused variable 'd2'}}
// autosar-warning@17 1 {{Unused variable 'd3'}}
// autosar-warning@9 1 {{There shall be no implicit floating-integral conversions}}
// autosar-warning@10 1 {{There shall be no implicit floating-integral conversions}}
// autosar-warning@12 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@13 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@15 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@16 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@17 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@5 1 {{Unused function 'test'}}
// autosar-warning@9 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@10 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@6 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@7 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@9 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@10 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@12 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@15 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@16 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@17 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
