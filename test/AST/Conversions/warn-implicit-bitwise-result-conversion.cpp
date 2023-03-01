// RUN: autocheck -verify -Wimplicit-bitwise-result-conversion %s

#include<cstdint>

namespace {
  void test1() {
    std::uint8_t value_8{static_cast<std::uint8_t>(0x3CU)};
    std::uint8_t result_8;
    std::uint16_t value_16{static_cast<std::uint16_t>(0x1B8AU)};
    std::uint16_t result_16;

    result_8 = (~value_8) >> 4U; // expected-warning {{If the bitwise operators ~ and << are applied to an operand with an underlying type of unsigned char or unsigned short, the result shall be immediately cast to the underlying type of the operand}}
    result_8 = (static_cast<std::uint8_t>(~value_8)) >> 4U;
    result_8 = (static_cast<std::int8_t>(~value_8)) >> 4U; // expected-warning {{If the bitwise operators ~ and << are applied to an operand with an underlying type of unsigned char or unsigned short, the result shall be immediately cast to the underlying type of the operand}}

    result_16 = ((value_8 << 4U) & value_16) >> 8U; // expected-warning {{If the bitwise operators ~ and << are applied to an operand with an underlying type of unsigned char or unsigned short, the result shall be immediately cast to the underlying type of the operand}}
    result_16 = (static_cast<std::uint16_t>(static_cast<std::uint16_t>(value_8) << 4U) & value_16) >> 8U;
  }

  std::uint8_t test2(std::uint8_t param_8) {
    return ~param_8; // expected-warning {{If the bitwise operators ~ and << are applied to an operand with an underlying type of unsigned char or unsigned short, the result shall be immediately cast to the underlying type of the operand}}
  }

  void test3() {
    std::uint8_t value_8 = 0x3CU;
    test2(~value_8); // expected-warning {{If the bitwise operators ~ and << are applied to an operand with an underlying type of unsigned char or unsigned short, the result shall be immediately cast to the underlying type of the operand}}
  }
}
