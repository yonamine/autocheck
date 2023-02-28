// RUN: autocheck -verify -Wimplicit-bitwise-binop-conversion %s

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
