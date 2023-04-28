// RUN: autocheck -verify -Wassignment-op-sub-expr %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@62 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@64 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@66 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@68 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@70 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@72 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@74 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@76 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@78 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@80 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@82 1 {{The condition of an if-statement and the condition of an iteration statement shall have type bool}}
// autosar-warning@33 1 {{Braced-initialization {}, without equals sign, shall be used for variable initialization}}
// autosar-warning@6 1 {{Unused function 'test'}}
// autosar-warning@88 1 {{C-style arrays shall not be used}}
// autosar-warning@33 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@85 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
