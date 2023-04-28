// RUN: autocheck -verify -Wimplicit-bitwise-result-conversion %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@4 1 {{There shall be no unused include directives: }}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@7 1 {{Unused function 'test1'}}
// autosar-warning@13 1 {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
// autosar-warning@14 1 {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
// autosar-warning@15 1 {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
// autosar-warning@15 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@15 1 {{Casts 'int' to 'uint8_t'}}
// autosar-warning@17 1 {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
// autosar-warning@18 1 {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
// autosar-warning@22 1 {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
// autosar-warning@27 1 {{Bitwise operators shall only be applied to operands of unsigned underlying type}}
// autosar-warning@25 1 {{Unused function 'test3'}}
// autosar-warning@17 1 {{Non-constant operands to a binary bitwise operator shall have the same underlying type}}
// autosar-warning@13 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@14 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@15 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@17 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@18 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@22 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@26 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@27 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@26 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@13 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@13 1 {{Casts 'int' to 'uint8_t'}}
// autosar-warning@14 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@14 1 {{Casts 'int' to 'uint8_t'}}
// autosar-warning@17 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@17 1 {{Casts 'int' to 'uint16_t'}}
// autosar-warning@18 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@18 1 {{Casts 'int' to 'uint16_t'}}
// autosar-warning@22 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@22 1 {{Casts 'int' to 'uint8_t'}}
// autosar-warning@27 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@27 1 {{Casts 'int' to 'uint8_t'}}
// autosar-warning@27 1 {{The value returned by a function having a non-void return type that is not an overloaded operator shall be used}}
// autosar-warning@8 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@10 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@26 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
