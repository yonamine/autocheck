// RUN: autocheck -verify -Wunsigned-literal-suffix %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@22 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@9 1 {{Unused function 'test'}}
// autosar-warning@15 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@14 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@14 1 {{Casts 'int' to 'uint32_t'}}
// autosar-warning@20 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@20 1 {{Casts 'int' to 'uint32_t'}}
// autosar-warning@24 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@24 1 {{Casts 'int' to 'uint32_t'}}
// autosar-warning@25 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@25 1 {{Casts 'int' to 'uint32_t'}}
// autosar-warning@27 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@27 1 {{Casts 'int' to 'uint32_t'}}
// autosar-warning@28 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@28 1 {{Casts 'int' to 'uint32_t'}}
// autosar-warning@11 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@18 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
