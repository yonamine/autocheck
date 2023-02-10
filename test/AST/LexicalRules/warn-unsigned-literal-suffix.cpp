// RUN: autocheck -verify -Wunsigned-literal-suffix %s
#include <cstdint>
namespace {
void func_with_unsigned(std::uint32_t) {}
#define MAX 0x10

void test() {

  std::uint32_t ua, ub, uc, ud;

  ua = 0U;
  ub = 0;           // expected-warning {{A "U" suffix shall be applied to all octal or hexadecimal integer literals of unsigned type}}
  uc = (std::uint32_t)0;
  ud = static_cast<std::uint32_t>(0);

  std::uint32_t u64_a{0U}, u32_a{0U}, u16_a{0U}, u8_a{0U};

  u8_a = MAX; // expected-warning {{A "U" suffix shall be applied to all octal or hexadecimal integer literals of unsigned type}}

  u64_a = u64_a + 0xFFFFFFFFFFFFFFFF;
  u32_a = u32_a + 0xFFFFFFFF;
  u16_a = u16_a + 0xFFFF; // expected-warning {{A "U" suffix shall be applied to all octal or hexadecimal integer literals of unsigned type}}
  u8_a = u8_a + 0xFF;     // expected-warning {{A "U" suffix shall be applied to all octal or hexadecimal integer literals of unsigned type}}

  func_with_unsigned(0x8000); // expected-warning {{A "U" suffix shall be applied to all octal or hexadecimal integer literals of unsigned type}}
  ua = ua + 0x8000;           // expected-warning {{A "U" suffix shall be applied to all octal or hexadecimal integer literals of unsigned type}}
}

} // namespace
