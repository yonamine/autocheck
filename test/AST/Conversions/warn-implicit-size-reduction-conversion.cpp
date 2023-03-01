// RUN: autocheck -verify -Wimplicit-size-reduction-conversion %s

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

// expected-warning@8 1 {{An implicit integral or floating-point conversion shall not reduce the size of the underlying type}}
