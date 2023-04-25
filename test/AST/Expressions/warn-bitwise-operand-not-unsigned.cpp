// RUN: autocheck -verify -Wbitwise-operand-not-unsigned %s

namespace {
void test() {
  int a{0};
  unsigned int b{0U};

  a = ~a; // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  b = ~b;

  a = a << 2; // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  a <<= 2;    // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  b = b << 2U;
  b <<= 2U;

  a = a >> 2; // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  a >>= 2;    // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  b = b >> 2U;
  b >>= 2U;

  a = a & a; // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  a &= a;    // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  b = b & b;
  b &= b;

  a = a | a; // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  a |= a;    // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  b = b | b;
  b |= b;

  a = a ^ a; // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  a ^= a;    // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  b = b ^ b;
  b ^= b;

  // bool signedness is implementation defined
  bool c{false};
  c = ~c;      // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c = c << 2U; // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c <<= 2U;    // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c = c >> 2U; // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c >>= 2U;    // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c = c & c;   // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c &= c;      // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c = c | c;   // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c |= c;      // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c = c ^ c;   // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
  c ^= c;      // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}

  unsigned char d;
  b = d | // This unsigned char is promoted to int which changes its sign. These implicit casts should not count as violations.
      0;  // expected-warning {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
}
} // namespace
