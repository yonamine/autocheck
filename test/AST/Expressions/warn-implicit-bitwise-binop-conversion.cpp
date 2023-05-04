// RUN: autocheck -verify -Wimplicit-bitwise-binop-conversion %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include<cstdint>

namespace {
  void test1() {
    std::uint8_t value_8 = 0U;
    std::uint16_t value_16{};

    value_16 = 0xABCDU ^ value_8;
    value_16 = value_16 ^ value_8; // expected-warning {{Non-constant operands to a binary bitwise operator shall have the same underlying type}}
    value_16 ^= value_8; // expected-warning {{Non-constant operands to a binary bitwise operator shall have the same underlying type}}

    value_16 = 0xABCDU & value_8;
    value_16 = value_16 & value_8; // expected-warning {{Non-constant operands to a binary bitwise operator shall have the same underlying type}}
    value_16 &= value_8; // expected-warning {{Non-constant operands to a binary bitwise operator shall have the same underlying type}}

    value_16 = 0xABCDU | value_8;
    value_16 = value_16 | value_8; // expected-warning {{Non-constant operands to a binary bitwise operator shall have the same underlying type}}
    value_16 |= value_8; // expected-warning {{Non-constant operands to a binary bitwise operator shall have the same underlying type}}

    value_16 = value_8 << 4U;
    value_16 = value_16 << value_8; // expected-warning {{Non-constant operands to a binary bitwise operator shall have the same underlying type}}
    value_16 <<= value_8; // expected-warning {{Non-constant operands to a binary bitwise operator shall have the same underlying type}}

    value_16 = value_8 >> 4U;
    value_16 = value_16 >> value_8; // expected-warning {{Non-constant operands to a binary bitwise operator shall have the same underlying type}}
    value_16 >>= value_8; // expected-warning {{Non-constant operands to a binary bitwise operator shall have the same underlying type}}

    unsigned long long x{};
    unsigned long long y{0UL};

    x = y & 1U;

  }
}

// autosar-warning@4 1 {{There shall be no unused include directives: }}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@7 1 {{Unused function 'test1'}}
// autosar-warning@23 1 {{If the bitwise operators ~ and << are applied to an operand with an underlying type of unsigned char or unsigned short, the result shall be immediately cast to the underlying type of the operand}}
// autosar-warning@24 1 {{If the bitwise operators ~ and << are applied to an operand with an underlying type of unsigned char or unsigned short, the result shall be immediately cast to the underlying type of the operand}}
// autosar-warning@11 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@12 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@15 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@16 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@19 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@20 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@23 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@24 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@27 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@28 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@8 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@31 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@32 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@8 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@12 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@12 1 {{Casts 'int' to 'uint16_t'}}
// autosar-warning@13 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@13 1 {{Casts 'uint8_t' to 'int'}}
// autosar-warning@16 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@16 1 {{Casts 'int' to 'uint16_t'}}
// autosar-warning@17 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@17 1 {{Casts 'uint8_t' to 'int'}}
// autosar-warning@20 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@20 1 {{Casts 'int' to 'uint16_t'}}
// autosar-warning@21 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@21 1 {{Casts 'uint8_t' to 'int'}}
// autosar-warning@23 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@23 1 {{Casts 'int' to 'uint16_t'}}
// autosar-warning@24 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@24 1 {{Casts 'int' to 'uint16_t'}}
// autosar-warning@25 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@25 1 {{Casts 'uint8_t' to 'int'}}
// autosar-warning@27 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@27 1 {{Casts 'int' to 'uint16_t'}}
// autosar-warning@28 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@28 1 {{Casts 'int' to 'uint16_t'}}
// autosar-warning@29 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@29 1 {{Casts 'uint8_t' to 'int'}}
// autosar-warning@8 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@32 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
