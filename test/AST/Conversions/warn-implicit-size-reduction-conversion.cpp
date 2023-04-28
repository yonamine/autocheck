// RUN: autocheck -verify -Wimplicit-size-reduction-conversion %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>

namespace {
void test() {
  std::int8_t value_8;
  std::int16_t value_16 = 0;
  std::int32_t value_32 = 0;
  std::int64_t value_64 = 0;

  value_8 = value_16;  // expected-warning {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
  value_8 = value_32;  // expected-warning {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
  value_8 = value_64;  // expected-warning {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
  value_16 = value_32; // expected-warning {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
  value_8 = value_64;  // expected-warning {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
  value_32 = value_64; // expected-warning {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}

  float f;
  double d = 0.0;
  f = d; // expected-warning {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
  static unsigned char testArray[] = {
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}; // expected-warning {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
  static unsigned char testArray2[] = {
      (unsigned char) 1, (unsigned char) 1, (unsigned char) 1, 1}; // expected-warning {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
  static unsigned char testArray3[] = {
      (unsigned char) 1, (unsigned char) 1, 1, (unsigned char) 1}; // expected-warning {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}

}
} // namespace

// expected-warning@9 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
// autosar-warning@4 1 {{There shall be no unused include directives: }}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@7 1 {{Unused function 'test'}}
// autosar-warning@9 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@10 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@11 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@21 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@23 1 {{C-style arrays shall not be used}}
// autosar-warning@23 1 {{Unused variable 'testArray'}}
// autosar-warning@23 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@23 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@25 1 {{C-style arrays shall not be used}}
// autosar-warning@25 1 {{Unused variable 'testArray2'}}
// autosar-warning@25 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@25 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@26 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@26 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@27 1 {{C-style arrays shall not be used}}
// autosar-warning@27 1 {{'testArray3'}}
// autosar-warning@27 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@27 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@28 1 {{Traditional C-style casts shall not be used}}
// autosar-warning@28 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@24 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@24 1 {{Casts 'int' to 'unsigned char'}}
// autosar-warning@26 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@26 1 {{Casts 'int' to 'unsigned char'}}
// autosar-warning@28 1 {{An implicit integral conversion shall not change the signedness of the underlying type}}
// autosar-note@28 1 {{Casts 'int' to 'unsigned char'}}
// autosar-warning@11 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@21 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@23 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@25 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@27 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
