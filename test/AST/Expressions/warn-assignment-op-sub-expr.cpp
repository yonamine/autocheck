// RUN: autocheck -verify -Wassignment-op-sub-expr %s

#include <cstdint>
namespace {
void test() {
  std::uint32_t x{};
  x = 1U;
  x += 1U;
  x -= 1U;
  x *= 1U;
  x /= 1U;
  x %= 1U;
  x &= 1U;
  x |= 1U;
  x ^= 1U;
  x <<= 1U;
  x >>= 1U;

  std::uint32_t y{};
  y = x = 1U;   // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  y = x += 1U;  // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  y = x -= 1U;  // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  y = x *= 1U;  // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  y = x /= 1U;  // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  y = x %= 1U;  // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  y = x &= 1U;  // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  y = x |= 1U;  // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  y = x ^= 1U;  // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  y = x <<= 1U; // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  y = x >>= 1U; // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}

  std::uint32_t z = x = y; // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  y = x = z;              // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}

  bool b{x != y};
  b = x == y;

  if ((x = y) != 0U) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x += y) != 0U) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x -= y) != 0U) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x *= y) != 0U) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x /= y) != 0U) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x %= y) != 0U) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x &= y) != 0U) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x |= y) != 0U) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x ^= y) != 0U) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x <<= y) != 0U) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x >>= y) != 0U) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }

  if ((x = y)) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x += y)) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x -= y)) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x *= y)) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x /= y)) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x %= y)) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x &= y)) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x |= y)) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x ^= y)) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x <<= y)) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }
  if ((x >>= y)) { // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
  }

  if (std::int32_t i{1}) {
  }

  std::int32_t array[3]{1, 2, 3};
  std::int32_t j;
  array[j = 1] = 2; // expected-warning 1 {{Assignment operators shall not be used in sub-expressions}}
}
} // namespace
